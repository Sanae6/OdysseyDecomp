#include <al/Library/LiveActor/ActorActionFunction.h>
#include <al/Library/LiveActor/ActorInitInfo.h>
#include <al/Library/LiveActor/ActorModelFunction.h>
#include <al/Library/LiveActor/ActorPoseKeeper.h>
#include <al/Library/LiveActor/ActorResourceFunction.h>
#include <al/Library/Math/MathUtil.h>
#include <al/Library/Nerve/NerveSetupUtil.h>
#include <al/Library/Obj/BreakModel.h>
#include <al/Library/Yaml/ByamlIter.h>
#include <al/Library/Yaml/ByamlUtil.h>

namespace {
MAKE_NERVE_NAMESPACE(BreakModel, Wait, al);
MAKE_NERVE_NAMESPACE(BreakModel, Break, al);
}

namespace al {
BreakModel::BreakModel(LiveActor const* rootActor, char const* objName, char const* modelName,
                       char const* fileSuffixName, const sead::Matrix34f* rootMtx, char const* breakActionName) : al::LiveActor(objName),
      mInitSuffix(fileSuffixName), mParent(rootActor), mRootMtx(rootMtx), mModelName(modelName), mBreakAction(breakActionName) {}

void BreakModel::init(const ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, mModelName, mInitSuffix);
    al::initNerve(this, &BreakModelNrvWait::sInstance, 0);
    al::invalidateClipping(this);

    al::ByamlIter actorInitFile;

    if(al::tryGetActorInitFileIter(&actorInitFile, this, "InitBreakModel", mInitSuffix)) {
        al::ByamlIter dither;
        if(actorInitFile.tryGetIterByKey(&dither, "DitherDisappear")) {
            mIsValid = al::tryGetByamlKeyBoolOrFalse(dither, "IsValid");
            if(mIsValid)
                mStep = al::getByamlKeyInt(dither, "Step");
        }
    }

    makeActorDead();
}

void BreakModel::appear(void) {
    if(mRootMtx)
        al::updatePoseMtx(this, mRootMtx);
    else
        al::copyPose(this, mParent);

    if(mBreakAction) {
        al::startAction(this, mBreakAction);
        if(!al::isNearZero(1.0f - mActionFrameRate, 0.001f))
            al::setActionFrameRate(this, mActionFrameRate);
    }
    al::setNerve(this, &BreakModelNrvBreak::sInstance);

    al::LiveActor::makeActorAlive();
}

void BreakModel::exeWait() {}

void BreakModel::exeBreak() {
    if(mIsValid) {
        if(mBreakAction) {

            f32 ditherStart = al::getActionFrameMax(this, mBreakAction) < mStep ?
                                  0.0f : al::getActionFrameMax(this, mBreakAction) - mStep;

            al::setModelAlphaMask(this, 1.0f - al::normalize((f32)al::getNerveStep(this), ditherStart,
                                                             al::getActionFrameMax(this, mBreakAction)));
        }
    }

    if(al::isActionEnd(this))
        kill();
}

}