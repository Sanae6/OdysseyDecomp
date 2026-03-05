#include "Scene/WorldWarpHoleScene.h"

#include <common/aglDrawContext.h>
#include <common/aglShaderEnum.h>
#include <gfx/seadViewport.h>

#include "Demo/ProjectDemoDirector.h"
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
#include "System/GameDataHolderAccessor.h"
#include "System/SequenceDemoSkipData.h"
#include "Util/DemoUtil.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL(WorldWarpHoleScene, Start)
NERVE_IMPL(WorldWarpHoleScene, Loop)
NERVE_IMPL(WorldWarpHoleScene, End)
NERVE_IMPL(WorldWarpHoleScene, SkipProc)

NERVES_MAKE_NOSTRUCT(WorldWarpHoleScene, Start, Loop, End, SkipProc)
}  // namespace

WorldWarpHoleScene::WorldWarpHoleScene(al::WipeHolder* wipeHolder)
    : al::Scene("WorldWarpHoleScene"), mWipeHolder(wipeHolder) {}

WorldWarpHoleScene::~WorldWarpHoleScene() {
    mDemoSyncedEventKeeper->finalize();
    mDemoActorHolder->kill();
    Application::instance()->getGameFramework()->enableClearRenderBuffer();
}

void WorldWarpHoleScene::init(const al::SceneInitInfo& initInfo) {
    initDrawSystemInfo(initInfo);

    mStageName = initInfo.initStageName;
    mScenarioNo = initInfo.scenarioNo;

    GameDataHolder* gameDataHolder = GameDataFunction::getGameDataHolder(initInfo.gameDataHolder);
    mGameDataHolder = gameDataHolder;
    GameDataFunction::startDemoStage(gameDataHolder, mStageName.cstr());
    initAndLoadStageResource(mStageName.cstr(), mScenarioNo);
    initSceneStopCtrl();
    initSceneObjHolder(SceneObjFactory::createSceneObjHolder());
    al::setSceneObj(this, GameDataFunction::getGameDataHolder(initInfo.gameDataHolder),
                    SceneObjID_GameDataHolder);

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

    rs::applyGameConfigData(this, mGameDataHolder->getGameConfigData());

    mLiveActorKit->getAreaObjDirector()->init(new ProjectAreaFactory());
    initLayoutKit(initInfo);
    al::LayoutInitInfo layoutInitInfo;
    al::initLayoutInitInfo(&layoutInitInfo, this, initInfo);
    al::PlacementInfo placementInfo;
    al::ActorInitInfo actorInitInfo;
    al::initActorInitInfo(&actorInitInfo, this, &placementInfo, &layoutInitInfo,
                          new ProjectActorFactory(), getSceneMsgCtrl(), initInfo.gameDataHolder);

    mDemoSyncedEventKeeper = new al::DemoSyncedEventKeeper(mAudioDirector);
    alDemoSyncedEventFunction::tryCreateAndSetSeKeeper(mDemoSyncedEventKeeper, mAudioDirector,
                                                       "WorldWarpHoleScene");
    mSceneAudioSystemPauseController = new SceneAudioSystemPauseController(mAudioDirector);

    al::initPadRumble(this, initInfo);

    initPlacement(actorInitInfo);

    initNerve(&Start, 1);
    mDemoSceneStateSkipDemo = new DemoSceneStateSkipDemo(this, initInfo,
                                                         !GameDataHolderAccessor(this)
                                                              ->getGameDataFile()
                                                              ->getSequenceDemoSkipData()
                                                              ->IsShowDemoWorldWarpHole(),
                                                         mSceneAudioSystemPauseController,
                                                         mDemoSyncedEventKeeper, mWipeHolder, true);
    al::initNerveState(this, mDemoSceneStateSkipDemo, &SkipProc, "デモスキップ");

    endInit(actorInitInfo);
}

void WorldWarpHoleScene::initPlacement(const al::ActorInitInfo& initInfo) {
    al::initGraphicsSystemInfo(this, mStageName.cstr(), 1);
    al::initPlacementAreaObj(this, initInfo);
    initPlacementSky(al::getStageInfoMap(this, 0), initInfo);
    for (s32 i = 0; i < al::getStageInfoMapNum(this); i++)
        WorldWarpHoleScene::initPlacementObject(al::getStageInfoMap(this, i), initInfo, nullptr);
    for (s32 i = 0; i < al::getStageInfoDesignNum(this); i++)
        WorldWarpHoleScene::initPlacementObject(al::getStageInfoDesign(this, i), initInfo, nullptr);
    for (s32 i = 0; i < al::getStageInfoSoundNum(this); i++)
        WorldWarpHoleScene::initPlacementObject(al::getStageInfoSound(this, i), initInfo, nullptr);

    const al::StageInfo* stageInfo = al::getStageInfoMap(this, 0);
    al::PlacementInfo demoObjList;
    if (al::tryGetPlacementInfo(&demoObjList, stageInfo, "DemoObjList")) {
        mDemoActorHolder = al::alDemoFunction::createDemoActorHolderForDemoScene(
            "DemoWorldWarpHoleStage", initInfo, nullptr, mScenarioNo);
    }
}

void WorldWarpHoleScene::appear() {
    al::Scene::appear();
    al::startBgm(this, "Stage", -1, 0);

    Application::instance()->getGameFramework()->disableClearRenderBuffer();

    al::setNearClipDistance(this, 1.0, 0);
    al::setFarClipDistance(this, 500000.0, 0);
}

void WorldWarpHoleScene::control() {
    if (al::isStopScene(this))
        return;
}

void WorldWarpHoleScene::drawMain() const {
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

    al::drawKit(this, "２Ｄ（デモ画面）");
    al::drawKit(this, "２Ｄベース（メイン画面）");
    al::drawKit(this, "２Ｄエフェクト（メイン画面）");
    al::drawKit(this, "２Ｄオーバー（メイン画面）");
}

void WorldWarpHoleScene::exeStart() {
    if (al::isFirstStep(this) && mDemoSceneStateSkipDemo->isNerveFirst(&Start)) {
        mDemoActorHolder->appear();
        mDemoActorHolder->startAction(0);
        al::addDemoActorFromDemoActorHolder(this, mDemoActorHolder);
        mDemoTimer = 0;

        mDemoName.format("ワールドワープホールデモ(%s, %s)", mScenarioNo == 1 ? "往路" : "復路",
                         "開始");

        auto demoName = mDemoName.cstr();
        rs::setDemoInfoDemoName(this, demoName);
        mDemoSyncedEventKeeper->startDemo(demoName);
    }

    DemoSceneFunction::updateKitWithDemoActorHolderSequence(this, mDemoActorHolder, nullptr,
                                                            nullptr, nullptr, nullptr);

    mDemoSyncedEventKeeper->updateOnlyWhenDemoRun(mDemoTimer);
    mDemoTimer += 1;

    mSceneAudioSystemPauseController->update();

    // if (mDemoEndFadeName)
    //     DemoSceneFunction::tryStartDemoEndFade(mWipeHolder, mDemoEndFadeName,
    //                                            mDemoSceneStateSkipDemo, mDemoActorHolder);

    if (mDemoSceneStateSkipDemo->tryChangeToSkipConfirmForLoadingScene(&Start, true))
        al::setNerve(this, &SkipProc);
    else if (mDemoActorHolder->isActionEndCamera(-1))
        al::setNerve(this, &Loop);
}

bool WorldWarpHoleScene::isEnableLoopEnd() const {
    return mLoopTimer > 180;
}

void WorldWarpHoleScene::exeSkipProc() {
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

void WorldWarpHoleScene::initPlacementSky(const al::StageInfo* stageInfo,
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

void WorldWarpHoleScene::initPlacementObject(const al::StageInfo* stageInfo,
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

void WorldWarpHoleScene::initPlacementDemo(const al::StageInfo* stageInfo,
                                           const al::ActorInitInfo& initInfo) {
    al::PlacementInfo demoObjList;
    if (al::tryGetPlacementInfo(&demoObjList, stageInfo, "DemoObjList")) {
        mDemoActorHolder = al::alDemoFunction::createDemoActorHolderForDemoScene(
            "DemoWorldWarpHoleStage", initInfo, nullptr, mScenarioNo);
    }
}
