#include <al/Library/Base/String.h>
#include <al/Library/LiveActor/ActorFlagFunction.h>
#include <al/Library/LiveActor/ActorInitInfo.h>
#include <al/Library/LiveActor/ActorModelFunction.h>
#include <al/Library/LiveActor/ActorPoseKeeper.h>
#include <al/Library/LiveActor/ActorResourceFunction.h>
#include <al/Library/LiveActor/SubActorFunction.h>
#include <al/Library/LiveActor/SubActorKeeper.h>
#include <al/Library/MapObj/MtxUtil.h>
#include <al/Library/Math/MathUtil.h>
#include <al/Library/Obj/PartsModel.h>
#include <al/Library/Yaml/ByamlUtil.h>

namespace al {
PartsModel::PartsModel(char const* name) : al::LiveActor(name) {}

void PartsModel::endClipped() {
    al::LiveActor::endClipped();
    updatePose();
}
void PartsModel::calcAnim() {
    updatePose();
    al::LiveActor::calcAnim();
}
void PartsModel::attackSensor(al::HitSensor* target, al::HitSensor* source) {
    mParentModel->attackSensor(target, source);
}
bool PartsModel::receiveMsg(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) {
    return mParentModel->receiveMsg(message, source, target);
}
void PartsModel::initPartsDirect(al::LiveActor* parent, al::ActorInitInfo const& initInfo, char const* arcName, sead::Matrix34f const* jointMtx,
                                 sead::Vector3f const& localTrans, sead::Vector3f const& localRotate, sead::Vector3f const& localScale, bool useFollowMtxScale) {
    mParentModel = parent;
    mJointMtx = jointMtx;
    mIsUseFollowMtxScale = useFollowMtxScale;
    al::initChildActorWithArchiveNameNoPlacementInfo(this, initInfo, arcName, nullptr);
    al::invalidateClipping(this);
    al::registerSubActor(parent, this);
    makeActorAlive();
    mIsUseLocalPos = true;
    mLocalTrans = localTrans;
    mLocalRotate = localRotate;
    mLocalScale = localScale;
}
void PartsModel::initPartsSuffix(al::LiveActor* parent, al::ActorInitInfo const& initInfo, char const* arcName,
                                 char const* suffix, sead::Matrix34f const* jointMtx, bool useFollowMtxScale) {
    mParentModel = parent;
    mJointMtx = jointMtx;
    mIsUseFollowMtxScale = useFollowMtxScale;
    al::initChildActorWithArchiveNameNoPlacementInfo(this, initInfo, arcName, suffix);
    al::invalidateClipping(this);
    al::registerSubActor(parent, this);
    makeActorAlive();
}
void PartsModel::initPartsMtx(al::LiveActor* parent, al::ActorInitInfo const& initInfo, char const* arcName,
                              sead::Matrix34f const* jointMtx, bool useFollowMtxScale) {
    mParentModel = parent;
    mJointMtx = jointMtx;
    mIsUseFollowMtxScale = useFollowMtxScale;
    al::initChildActorWithArchiveNameNoPlacementInfo(this, initInfo, arcName, nullptr);
    al::invalidateClipping(this);
    al::registerSubActor(parent, this);
    makeActorAlive();
}
void PartsModel::initPartsFixFile(al::LiveActor* parent, al::ActorInitInfo const& initInfo, char const* arcName, char const* arcSuffix, char const* suffix) {
    initPartsFixFileNoRegister(parent, initInfo, arcName, arcSuffix, suffix);
    al::registerSubActor(parent, this);
}
void PartsModel::initPartsFixFileNoRegister(al::LiveActor* parent, al::ActorInitInfo const& initInfo, char const* arcName, char const* arcSuffix, char const* suffix) {

    mParentModel = parent;
    mJointMtx = parent->getBaseMtx();

    al::initChildActorWithArchiveNameNoPlacementInfo(this, initInfo, arcName, arcSuffix);
    al::invalidateClipping(this);

    sead::FixedSafeString<0x80> initArcName;
    al::createFileNameBySuffix(&initArcName, "InitPartsFixInfo", suffix);

    if(!al::isExistModelResourceYaml(mParentModel, initArcName.cstr(), nullptr))
        return makeActorAlive();
    u8 *modelResByml = al::getModelResourceYaml(mParentModel, initArcName.cstr(), nullptr);
    al::ByamlIter modelResIter(modelResByml);

    const char *jointName = nullptr;
    modelResIter.tryGetStringByKey(&jointName, "JointName");

    if(jointName)
        mJointMtx = al::getJointMtxPtr(mParentModel, jointName);

    al::tryGetByamlV3f(&mLocalTrans, modelResIter, "LocalTrans");
    al::tryGetByamlV3f(&mLocalRotate, modelResIter, "LocalRotate");
    al::tryGetByamlV3f(&mLocalScale, modelResIter, "LocalScale");

    mIsUseLocalScale = al::tryGetByamlKeyBoolOrFalse(modelResIter, "UseLocalScale");

    if(!al::isNearZero(mLocalTrans, 0.001f) || !al::isNearZero(mLocalRotate, 0.001f) || mIsUseLocalScale)
        mIsUseLocalPos = true;

    mIsUseFollowMtxScale = al::tryGetByamlKeyBoolOrFalse(modelResIter, "UseFollowMtxScale");

    makeActorAlive();

}
// NON-MATCHING: needs to have proper matrix math implemented still
void PartsModel::updatePose() {
    sead::Matrix34f poseMtx;
    sead::Matrix34f jointMtx;

    if(!field_142)
        return;
    if(mIsUseLocalPos) {
        jointMtx = *mJointMtx;
        if(mIsUseFollowMtxScale) {
            sead::Vector3f mtxScale;
            al::calcMtxScale(&mtxScale, jointMtx);
            mtxScale *= 1.0f;
            al::setScale(this, mtxScale);
        }
        al::normalize(&jointMtx);
        al::updatePoseMtx(this, &jointMtx);
        return;
    }

    mLocalRotate *= 0.017453f;

    poseMtx.makeRT(mLocalRotate, mLocalTrans);

    if(mIsUseFollowMtxScale || mIsUseLocalScale) {

    }

}
void PartsModel::offSyncAppearAndHide() {
    al::offSyncAppearSubActor(mParentModel, this);
    al::offSyncHideSubActor(mParentModel, this);
}
void PartsModel::onSyncAppearAndHide() {
    al::onSyncHideSubActor(mParentModel, this);

    if (al::isHideModel(mParentModel))
        al::hideModelIfShow(this);
    else
        al::showModelIfHide(this);

    al::onSyncAppearSubActor(mParentModel, this);

    if (al::isDead(mParentModel) && al::isAlive(this))
        makeActorDead();
    else if (al::isAlive(mParentModel) && al::isDead(this))
        makeActorAlive();

}
}