#include <al/Library/LiveActor/ActorInitInfo.h>
#include <al/Library/LiveActor/ActorModelFunction.h>
#include <al/Library/LiveActor/ActorPoseKeeper.h>
#include <al/Library/Math/MathUtil.h>
#include <game/MapObj/WorldMapParts.h>

void localFunc_34B9A8(WorldMapParts *mapParts) {
    al::invalidateOcclusionQuery(mapParts);
//    if(al::isExistSubActorKeeper(mapParts) && al::getSubActorNum(mapParts) >= 1) {
//
//    }
}

WorldMapParts::WorldMapParts(char const* name) : al::LiveActor(name) {}

void WorldMapParts::setWorldMtx(sead::Matrix34f const& srcMtx) {
    sead::Matrix34f copyMtx;
    sead::Matrix34f inverse;

    inverse.setInverse(*mWorldMtx);
    copyMtx.setMul(inverse, srcMtx);
    setLocalMtx(copyMtx);
}

void WorldMapParts::updatePose(void) {
    sead::Matrix34f prevWorldMtx = *mWorldMtx;
    al::normalize(&prevWorldMtx);

    prevWorldMtx.setMul(prevWorldMtx, mLocalMtx);

    al::updatePoseMtx(this, &prevWorldMtx);

}

void WorldMapParts::control(void) {
    updatePose();
}

void WorldMapParts::setLocalMtx(sead::Matrix34f const& srcMtx) {
    mLocalMtx = srcMtx;
}

void WorldMapParts::initParts(WorldMapParts* mapParts,char const* arcName,al::ActorInitInfo const& initInfo,
                              sead::Matrix34f const* worldMtx, sead::Matrix34f const& localMtx, char const* suffix) {
    al::initChildActorWithArchiveNameNoPlacementInfo(mapParts, initInfo, arcName, suffix);
    mapParts->mWorldMtx = worldMtx;
    mapParts->setWorldMtx(localMtx);
    mapParts->updatePose();

    if(al::isExistModel(mapParts))
        localFunc_34B9A8(mapParts);

    mapParts->makeActorDead();
}

WorldMapParts* WorldMapParts::create(char const* name,char const* arcName,al::ActorInitInfo const& initInfo,
                                     sead::Matrix34f const* worldMtx, sead::Matrix34f const& localMtx, char const* suffix) {

    WorldMapParts *newParts = new WorldMapParts(name);

    initParts(newParts, arcName, initInfo, worldMtx, localMtx, suffix);

    return newParts;
}