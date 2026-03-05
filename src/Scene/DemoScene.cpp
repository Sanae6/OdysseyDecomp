#include "Scene/DemoScene.h"

#include <common/aglDrawContext.h>
#include <common/aglShaderEnum.h>
#include <gfx/seadViewport.h>

#include "Library/Area/AreaObjDirector.h"
#include "Library/Audio/AudioDirectorInitInfo.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Demo/DemoActorHolder.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/Demo/DemoSyncedEventKeeper.h"
#include "Library/Draw/GraphicsFunction.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Draw/ViewRenderer.h"
#include "Library/Framework/GameFrameworkNx.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Layout/LayoutTextureRenderer.h"
#include "Library/Layout/LayoutUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Projection/Projection.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shader/DeferredRendering/SamplerLocation.h"
#include "Library/Stage/StageResourceList.h"
#include "Library/System/GameSystemInfo.h"
#include "Library/Yaml/ByamlIter.h"

#include "Audio/DemoSoundSynchronizer.h"
#include "Demo/ProjectDemoDirector.h"
#include "Scene/DemoSceneFunction.h"
#include "Scene/DemoSceneStateSkipDemo.h"
#include "Scene/ProjectActorFactory.h"
#include "Scene/ProjectAreaFactory.h"
#include "Scene/ProjectCameraPoserFactory.h"
#include "Scene/SceneAudioSystemPauseController.h"
#include "Scene/SceneObjFactory.h"
#include "System/Application.h"
#include "System/GameConfigData.h"
#include "System/GameDataFunction.h"
#include "Util/DemoUtil.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL(DemoScene, Play)
NERVE_IMPL(DemoScene, SkipProc)

NERVES_MAKE_NOSTRUCT(DemoScene, Play, SkipProc)
}  // namespace

DemoScene::DemoScene(al::WipeHolder* wipeHolder)
    : al::Scene("DemoScene"), mWipeHolder(wipeHolder) {}

DemoScene::~DemoScene() {
    mDemoSyncedEventKeeper->finalize();
    mDemoActorHolder->kill();
    Application::instance()->getGameFramework()->enableClearRenderBuffer();
    if (mLayoutTextureRenderer) {
        delete mLayoutTextureRenderer;
        mLayoutTextureRenderer = nullptr;
    }
}

void DemoScene::init(const al::SceneInitInfo& initInfo) {
    initDrawSystemInfo(initInfo);

    mStageName = initInfo.initStageName;
    mScenarioNo = initInfo.scenarioNo;

    initAndLoadStageResource(mStageName.cstr(), mScenarioNo);
    initSceneObjHolder(SceneObjFactory::createSceneObjHolder());

    GameDataHolder* gameDataHolder = GameDataFunction::getGameDataHolder(initInfo.gameDataHolder);
    mGameDataHolder = gameDataHolder;
    al::setSceneObj(this, mGameDataHolder, SceneObjID_GameDataHolder);
    GameDataFunction::startDemoStage(this, mStageName.cstr());

    al::LayoutTextureRenderer* layoutTextureRenderer = new al::LayoutTextureRenderer();
    mLayoutTextureRenderer = layoutTextureRenderer;
    al::setSceneObj(this, mLayoutTextureRenderer, SceneObjID_alLayoutTextureRenderer);

    initSceneStopCtrl();

    al::GraphicsInitArg initArg(al::getSceneDrawContext(this),
                                al::getSceneFrameBufferConsole(this));
    initArg._3c = 2;
    initArg._10 = 2 << (initArg.atmosScatterViewNum & 0x1f);
    initArg._6 = true;
    initLiveActorKitWithGraphics(initArg, initInfo, 0x400, 1, 2);
    alGraphicsFunction::invalidateCameraBlurSystem(this);
    mLiveActorKit->getGraphicsSystemInfo()->set_2f4(2);

    mLiveActorKit->initHitSensorDirector();
    ProjectDemoDirector* demoDirector = new ProjectDemoDirector(
        mLiveActorKit->getPlayerHolder(), mLiveActorKit->getGraphicsSystemInfo());
    mLiveActorKit->setDemoDirector(demoDirector);

    al::AudioDirectorInitInfo audioDirectorInitInfo;
    al::initAudioDirector3D(this, initInfo, audioDirectorInitInfo);
    alAudioSystemFunction::disableAudioMaximizer(initInfo.gameSystemInfo);
    al::initSceneAudioKeeper(this, initInfo, nullptr);

    al::initCameraDirectorWithoutStageResource(this, new ProjectCameraPoserFactory());
    al::setCameraAspect(this, al::getSceneFrameBufferMainAspect(this), -1.0f);

    al::initNatureDirector(this);

    rs::applyGameConfigData(this, mGameDataHolder->getGameConfigData());

    mLiveActorKit->getAreaObjDirector()->init(new ProjectAreaFactory());
    initLayoutKit(initInfo);
    al::LayoutInitInfo layoutInitInfo;
    al::initLayoutInitInfo(&layoutInitInfo, this, initInfo);
    al::PlacementInfo placementInfo;
    al::ActorInitInfo actorInitInfo;
    al::initActorInitInfo(&actorInitInfo, this, &placementInfo, &layoutInitInfo,
                          new ProjectActorFactory(), getSceneMsgCtrl(), initInfo.gameDataHolder);
    initLayout(layoutInitInfo);

    mDemoSyncedEventKeeper = new al::DemoSyncedEventKeeper(mAudioDirector);
    alDemoSyncedEventFunction::tryCreateAndSetSeKeeper(mDemoSyncedEventKeeper, mAudioDirector,
                                                       "DemoScene");
    mSceneAudioSystemPauseController = new SceneAudioSystemPauseController(mAudioDirector);
    mDemoSoundSynchronizer = new DemoSoundSynchronizer(
        initInfo.gameSystemInfo->applicationMessageReciever, mAudioDirector);

    al::initPadRumble(this, initInfo);

    initPlacement(actorInitInfo);

    initNerve(&Play, 1);
    mDemoSceneStateSkipDemo =
        new DemoSceneStateSkipDemo(this, initInfo, true, mSceneAudioSystemPauseController,
                                   mDemoSyncedEventKeeper, mWipeHolder, true);
    al::initNerveState(this, mDemoSceneStateSkipDemo, &SkipProc, "デモスキップ");

    if (al::isEqualString(mStageName.cstr(), "DemoOpeningStage"))
        mDemoEndFadeName = "オープニング";
    else if (al::isEqualString(mStageName.cstr(), "DemoCrashHomeFallStage"))
        mDemoEndFadeName = "ホーム陥落";
    else if (al::isEqualString(mStageName.cstr(), "DemoLavaWorldScenario1EndStage"))
        mDemoEndFadeName = "溶岩シナリオ1終了";

    endInit(actorInitInfo);
}

void DemoScene::initPlacement(const al::ActorInitInfo& initInfo) {
    al::initGraphicsSystemInfo(this, mStageName.cstr(), mScenarioNo);
    al::initPlacementAreaObj(this, initInfo);
    initPlacementSky(al::getStageInfoMap(this, 0), initInfo);
    al::initPlacementObjectMap(this, initInfo, "NatureList");
    al::initPlacementObjectMap(this, initInfo, "ObjectList");
    al::initPlacementObjectDesign(this, initInfo, "ObjectList");
    al::initPlacementObjectSound(this, initInfo, "ObjectList");

    const al::StageInfo* stageInfo = al::getStageInfoMap(this, 0);
    // inline of initPlacementDemo(stageInfo, initInfo);
    al::PlacementInfo demoObjList;
    if (al::tryGetPlacementInfo(&demoObjList, stageInfo, "DemoObjList")) {
        mDemoActorHolder = al::alDemoFunction::createDemoActorHolder(mStageName.cstr(), initInfo,
                                                                     nullptr, mScenarioNo, true);
    }
}

void DemoScene::appear() {
    al::Scene::appear();
    Application::instance()->getGameFramework()->disableClearRenderBuffer();

    al::setNearClipDistance(this, 10.0, 0);
    al::setFarClipDistance(this, 500000.0, 0);

    mDemoSoundSynchronizer->startSync();
    al::startBgm(this, "Stage", -1, 0);
}

void DemoScene::control() {
    if (al::isStopScene(this))
        return;

    mDemoSoundSynchronizer->update();
    mDemoSoundSynchronizer->trySync(mDemoTimer, mDemoSyncedEventKeeper);
}

void DemoScene::kill() {
    al::Scene::kill();

    mDemoSoundSynchronizer->endSync();
}

void DemoScene::drawMain() const {
    mLayoutTextureRenderer->drawToTexture();

    agl::DrawContext* drawContext = al::getSceneDrawContext(this);
    agl::RenderBuffer* renderBuffer = al::getSceneFrameBufferMain(this);

    sead::Viewport viewport(*renderBuffer);
    viewport.apply(drawContext, *renderBuffer);

    al::ViewRenderer* viewRenderer = mLiveActorKit->getGraphicsSystemInfo()->getViewRenderer();
    const sead::LookAtCamera& lookAtCamera = al::getLookAtCamera(this, 0);
    const al::Projection& projection = al::getProjection(this, 0);

    al::tryChangeShaderMode(drawContext, agl::ShaderMode(4));

    viewRenderer->drawView(0, mDrawSystemInfo, projection, lookAtCamera, renderBuffer, viewport,
                           true, false, false);

    al::setRenderBuffer(mLayoutKit, renderBuffer);

    if (rs::isModeE3MovieRom()) {
        al::drawKit(this, "２Ｄ（撮影用）");
    } else {
        al::drawKit(this, "２Ｄ（デモ画面）");
        al::drawKit(this, "２Ｄベース（メイン画面）");
        al::drawKit(this, "２Ｄエフェクト（メイン画面）");
        al::drawKit(this, "２Ｄオーバー（メイン画面）");
    }
}

static struct Entry {
    const char* key;
    const char* value;
} entries[] = {
    {"DemoOpeningStage", "オープニングデモ"},
    {"DemoLavaWorldScenario1EndStage", "溶岩のワールドシナリオ1終了デモ"},
    {"DemoEndingStage", "エンディングデモ"},
    {"DemoCrashHomeFallStage", "クッパの攻撃～ホーム墜落デモ"},
    {"DemoCrashHomeStage", "ワールド間移動デモ(クッパ戦艦発見)"},
    {"DemoBossRaidAttackStage", "ワールド間移動デモ(襲撃ボス攻撃)"},
};

static inline s32 getDemoNameIndex(const char* name) {
    if (name == nullptr)
        return -1;
    else if (al::isEqualString(name, entries[0].key))
        return 0;
    else if (al::isEqualString(name, entries[1].key))
        return 1;
    else if (al::isEqualString(name, entries[2].key))
        return 2;
    else if (al::isEqualString(name, entries[3].key))
        return 3;
    else if (al::isEqualString(name, entries[4].key))
        return 4;
    else if (al::isEqualString(name, entries[5].key))
        return 5;
    return -1;
}

void DemoScene::exePlay() {
    if (al::isFirstStep(this) && mDemoSceneStateSkipDemo->isNerveFirst(&Play)) {
        alSeFunction::startListenerPoser(mAudioDirector, "デモ", 0);
        mDemoSceneStateSkipDemo->appearPlayGuideSkipForLoadingScene();

        if (mDemoActorHolder) {
            mDemoActorHolder->startSequence();
            al::addDemoActorFromDemoActorHolder(this, mDemoActorHolder);
        }

        mDemoTimer = 0;
        const char* stageName = mStageName.cstr();
        auto demoName = getDemoNameIndex(stageName);
        if (demoName == -1)
            mDemoName = nullptr;
        else {
            mDemoName = entries[demoName].value;
            rs::setDemoInfoDemoName(this, entries[demoName].value);
            mDemoSyncedEventKeeper->startDemo(mDemoName);
        }
    }

    DemoSceneFunction::updateKitWithDemoActorHolderSequence(this, mDemoActorHolder, nullptr,
                                                            nullptr, nullptr, nullptr);

    mDemoSyncedEventKeeper->updateOnlyWhenDemoRun(mDemoTimer);
    mDemoTimer += 1;

    mSceneAudioSystemPauseController->update();

    if (mDemoEndFadeName)
        DemoSceneFunction::tryStartDemoEndFade(mWipeHolder, mDemoEndFadeName,
                                               mDemoSceneStateSkipDemo, mDemoActorHolder);

    if (mDemoSceneStateSkipDemo->tryChangeToSkipConfirmForLoadingScene(&Play, isLoadEnd())) {
        mDemoSceneStateSkipDemo->setDemoActorHolder(mDemoActorHolder);
        al::setNerve(this, &SkipProc);
        return;
    }

    if (mDemoActorHolder->isEndSequence())
        kill();
}

bool DemoScene::isLoadEnd() const {
    return mLoadEnded;
}

void DemoScene::exeSkipProc() {
    al::isFirstStep(this);

    if (!al::updateNerveState(this))
        return;

    al::updateKitListPostOnNerveEnd(this);

    if (mDemoSceneStateSkipDemo->skipped()) {
        alAudioSystemFunction::pauseAudioDirector(mAudioDirector, false, 0);
        alAudioSystemFunction::stopAllSound(mAudioDirector, 0, false);
        kill();
        return;
    }

    al::setNerve(this, mDemoSceneStateSkipDemo->firstNerve());
}

void DemoScene::initLayout(const al::LayoutInitInfo& initInfo) {}

void DemoScene::initPlacementSky(const al::StageInfo* stageInfo,
                                 const al::ActorInitInfo& initInfo) {
    al::PlacementInfo list;
    s32 count = 0;
    al::ByamlIter listIter;
    al::tryGetPlacementInfoAndCount(&list, &count, stageInfo, "SkyList");
    for (s32 i = 0; i < count; i++) {
        al::PlacementInfo actorInfo;
        al::getPlacementInfoByIndex(&actorInfo, list, i);
        al::createPlacementActorFromFactory(initInfo, &actorInfo);
    }
}

void DemoScene::initPlacementObject(const al::StageInfo* stageInfo,
                                    const al::ActorInitInfo& initInfo, const char*) {
    al::PlacementInfo list;
    s32 count = 0;
    al::tryGetPlacementInfoAndCount(&list, &count, stageInfo, "ObjectList");
    for (s32 i = 0; i < count; i++) {
        al::PlacementInfo actorInfo;
        al::getPlacementInfoByIndex(&actorInfo, list, i);
        const char* name = nullptr;
        al::getObjectName(&name, actorInfo);
        al::createPlacementActorFromFactory(initInfo, &actorInfo);
    }
}

void DemoScene::initPlacementDemo(const al::StageInfo* stageInfo,
                                  const al::ActorInitInfo& initInfo) {
    al::PlacementInfo demoObjList;
    if (al::tryGetPlacementInfo(&demoObjList, stageInfo, "DemoObjList")) {
        mDemoActorHolder = al::alDemoFunction::createDemoActorHolder(mStageName.cstr(), initInfo,
                                                                     nullptr, mScenarioNo, true);
    }
}
