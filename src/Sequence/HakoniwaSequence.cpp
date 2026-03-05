#include "Sequence/HakoniwaSequence.h"

#include <common/aglDrawContext.h>
#include <common/aglRenderBuffer.h>
#include <gfx/seadViewport.h>

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutUtil.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Play/Layout/WipeHolder.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Sequence/Sequence.h"
#include "Library/System/GameSystemInfo.h"

#include "Layout/LoadLayoutCtrl.h"
#include "ModeBalloon/TimeBalloonSequenceInfo.h"
#include "Scene/StageScene.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataHolder.h"
#include "System/SaveDataAccessFunction.h"
#include "Util/StageInputFunction.h"
#include "math/seadMathCalcCommon.h"

namespace {
NERVE_IMPL(HakoniwaSequence, PlayStage)
NERVE_IMPL(HakoniwaSequence, ChangeLanguage)
NERVE_IMPL(HakoniwaSequence, BootLoadData)
NERVE_IMPL(HakoniwaSequence, DemoOpening)
NERVE_IMPL(HakoniwaSequence, DemoEnding)
NERVE_IMPL(HakoniwaSequence, DemoWorldWarp)
NERVE_IMPL(HakoniwaSequence, DemoLava)
NERVE_IMPL(HakoniwaSequence, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToEnding, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToDemoLava, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToMiss, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToReboot, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToModeChange, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToNewGame, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToChangeLanguage, Destroy)
NERVE_IMPL_(HakoniwaSequence, DestroyToDebugLoad, Destroy)
NERVE_IMPL(HakoniwaSequence, LoadStage)
NERVE_IMPL_(HakoniwaSequence, LoadStageAfterOpening, LoadStage)
NERVE_IMPL(HakoniwaSequence, LoadWorldResource)
NERVE_IMPL(HakoniwaSequence, Miss)
NERVE_IMPL(HakoniwaSequence, WaitWriteData)
NERVE_IMPL(HakoniwaSequence, WaitWriteDataModeChange)
NERVE_IMPL(HakoniwaSequence, FadeToNewGame)
NERVE_IMPL(HakoniwaSequence, MissCoinSub)
NERVE_IMPL(HakoniwaSequence, MissEnd)
NERVE_IMPL(HakoniwaSequence, WaitLoadData)
NERVE_IMPL(HakoniwaSequence, WaitLoadDataModeChange)

NERVES_MAKE_STRUCT(HakoniwaSequence, PlayStage, ChangeLanguage, BootLoadData, DemoOpening,
                   DemoEnding, DemoWorldWarp, DemoLava, Destroy, DestroyToEnding, DestroyToDemoLava,
                   DestroyToMiss, DestroyToReboot, DestroyToModeChange, DestroyToNewGame,
                   DestroyToChangeLanguage, DestroyToDebugLoad, LoadStage, LoadStageAfterOpening,
                   LoadWorldResource, Miss, WaitWriteData, WaitWriteDataModeChange, FadeToNewGame,
                   MissCoinSub, WaitLoadData, WaitLoadDataModeChange)

// which ones are struct and which ones are no struct is not yet clear :(
NERVES_MAKE_NOSTRUCT(HakoniwaSequence, MissEnd)
}  // namespace

HakoniwaSequence::HakoniwaSequence(const char* name) : al::Sequence(name) {
    mStageName.clear();
}

void HakoniwaSequence::update() {
    mLoadLayoutCtrl->updateNerve();

    al::Sequence::update();

    if (mGameDataHolder->isRequireSave() && !al::isNerve(this, &NrvHakoniwaSequence.Destroy) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToEnding) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToDemoLava) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToMiss) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToDebugLoad) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToModeChange) &&
        !al::isNerve(this, &NrvHakoniwaSequence.DestroyToReboot) && isEnableSave())
        SaveDataAccessFunction::startSaveDataWrite(mGameDataHolder);

    SaveDataAccessFunction::updateSaveDataAccess(mGameDataHolder, false);

    al::executeUpdate(mLayoutKit);

    al::Scene* scene = getCurrentScene();
    if (scene == nullptr || !mWipeHolder->isOpenEnd())
        rs::startWipeOpen(mGameDataHolder);
    else
        rs::endWipeOpen(mGameDataHolder);

    if (mBalloonSeqInfo != nullptr)
        mBalloonSeqInfo->updateBalloonFind();
}

void HakoniwaSequence::drawMain() const {
    al::Sequence::drawMain();
    al::DrawSystemInfo* info = getDrawInfo();
    agl::DrawContext* context = info->drawContext;
    const agl::RenderBuffer* buffer = info->dockedRenderBuffer;
    if (!info->isDocked)
        buffer = info->handheldRenderBuffer;

    mScreenCaptureExecutor->tryCaptureAndDraw(context, buffer, 0);
    sead::Viewport viewport{*buffer};
    viewport.apply(context, *buffer);
    buffer->bind(context);
    al::setRenderBuffer(mLayoutKit, buffer);
    al::executeDraw(mLayoutKit, "２Ｄバック（メイン画面）");
    al::executeDraw(mLayoutKit, "２Ｄベース（メイン画面）");
    al::executeDraw(mLayoutKit, "２Ｄオーバー（メイン画面）");
}

void HakoniwaSequence::updatePadSystem() {
    if (ControllerAppletFunction::tryReconnectGamePad(mGamePadSystem))
        return;

    if (mGameDataHolder->isSeparatePlay() &&
        ControllerAppletFunction::connectControllerSinglePlay(mGamePadSystem))
        mGameDataHolder->setSeparatePlay(false);
}

void HakoniwaSequence::destroySceneHeap(bool destroyResource) {
    al::destroySceneHeap(destroyResource);
}

bool HakoniwaSequence::isEnableSave() const {
    if (!SaveDataAccessFunction::isEnableSave(mGameDataHolder) ||
        !al::isNerve(this, &NrvHakoniwaSequence.PlayStage))
        return false;
    StageScene* scene = (StageScene*)getCurrentScene();
    return scene->isEnableSave();
}

bool HakoniwaSequence::isDisposable() const {
    return al::isNerve(this, &NrvHakoniwaSequence.PlayStage) ||
           al::isNerve(this, &NrvHakoniwaSequence.ChangeLanguage);
}

al::Scene* HakoniwaSequence::getCurrentScene() const {
    return mCurrentScene;
}

void HakoniwaSequence::exeMissCoinSub() {
    if (al::isFirstStep(this))
        al::startAction(mCounterMiss, "SubtractColor", nullptr);

    if (al::isIntervalStep(this, 4, 0)) {
        mCurrentCoins = sead::Mathi::max(mCurrentCoins - 1, 0);

        al::setPaneStringFormat(mCounterMiss, "TxtCounter",
                                al::StringTmp<64>("%04d", mCurrentCoins).cstr());

        al::requestCaptureRecursive(mCounterMiss);
    }

    if (mCurrentCoins == mFinalCoins) {
        al::startAction(mCounterMiss, "Subtract");
        al::setNerve(this, &MissEnd);
    }
}
