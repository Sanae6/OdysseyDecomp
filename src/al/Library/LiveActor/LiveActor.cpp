#include <al/Library/LiveActor/LiveActor.h>

#include <al/Library/Light/ActorPrepassLightKeeper.h>
#include <al/Library/LiveActor/ActorFlagFunction.h>
#include <al/Library/LiveActor/ActorModelFunction.h>
#include <al/Library/Rail/RailKeeper.h>
#include <al/Library/Shadow/ShadowKeeper.h>
#include <string.h>

namespace al {

LiveActor::LiveActor(const char* actorName) : mActorName(actorName) {
    memset(&mPoseKeeper, 0, 0xB8);
    mFlags = new LiveActorFlag();
    mShadowKeeper = new ShadowKeeper();
}

al::NerveKeeper* LiveActor::getNerveKeeper() const {
    return mNerveKeeper;
}

const char* LiveActor::getName() const {
    return mActorName;
}
al::EffectKeeper* LiveActor::getEffectKeeper() const {
    return mEffectKeeper;
}
AudioKeeper* LiveActor::getAudioKeeper() const {
    return mAudioKeeper;
}
StageSwitchKeeper* LiveActor::getStageSwitchKeeper() const {
    return mStageSwitchKeeper;
}

void LiveActor::init(const al::ActorInitInfo&) {}
void LiveActor::attackSensor(al::HitSensor*, al::HitSensor*) {}
bool LiveActor::receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) {
    return 0;
}
bool LiveActor::receiveMsgScreenPoint(const al::SensorMsg*, al::ScreenPointer*, al::ScreenPointTarget*) {
    return 0;
}
void LiveActor::control() {}
void LiveActor::draw() const {}

void LiveActor::initAfterPlacement() {
    al::tryInitFixedModelGpuBuffer(this);
    if (mActorPrePassLightKeeper)
        mActorPrePassLightKeeper->initAfterPlacement();
}

void LiveActor::appear() {
    makeActorAlive();
}
void LiveActor::kill() {
    makeActorDead();
}
RailRider* LiveActor::getRailRider() const {
    if (mRailKeeper)
        return mRailKeeper->getRailRider();
    return nullptr;
}
ActorSceneInfo* LiveActor::getSceneInfo() const {
    return mSceneInfo;
}
void LiveActor::initPoseKeeper(ActorPoseKeeperBase* poseKeeper) {
    mPoseKeeper = poseKeeper;
}
void LiveActor::initExecuteInfo(ActorExecuteInfo* executeInfo) {
    mExecuteInfo = executeInfo;
}
void LiveActor::initModelKeeper(ModelKeeper* modelKeeper) {
    mModelKeeper = modelKeeper;
    al::offUpdateMovementEffectAudioCollisionSensor(this);
}
void LiveActor::initActionKeeper(ActorActionKeeper* actionKeeper) {
    mActorActionKeeper = actionKeeper;
}
void LiveActor::initNerveKeeper(NerveKeeper* nerveKeeper) {
    mNerveKeeper = nerveKeeper;
}
void LiveActor::initAudioKeeper(AudioKeeper* audioKeeper) {
    mAudioKeeper = audioKeeper;
}
void LiveActor::initActorPrePassLightKeeper(ActorPrePassLightKeeper* lightKeeper) {
    mActorPrePassLightKeeper = lightKeeper;
}
void LiveActor::initActorOcclusionKeeper(ActorOcclusionKeeper* occlusionKeeper) {
    mActorOcclusionKeeper = occlusionKeeper;
}
void LiveActor::initSubActorKeeper(SubActorKeeper* subActorKeeper) {
    mSubActorKeeper = subActorKeeper;
}
void LiveActor::initSceneInfo(ActorSceneInfo* sceneInfo) {
    mSceneInfo = sceneInfo;
}

LiveActorFlag::LiveActorFlag() = default;

};  // namespace al
