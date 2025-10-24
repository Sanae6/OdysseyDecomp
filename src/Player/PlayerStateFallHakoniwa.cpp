#include "Player/PlayerStateFallHakoniwa.h"

#include <math/seadMatrix.hpp>

#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"

#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerJudgeInvalidateInputFall.h"
#include "Player/PlayerTrigger.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerUtil.h"
#include "math/seadMathCalcCommon.h"
#include "math/seadVectorFwd.h"

namespace {
NERVE_IMPL(PlayerStateFallHakoniwa, Fall);

NERVES_MAKE_NOSTRUCT(PlayerStateFallHakoniwa, Fall);
}  // namespace

PlayerStateFallHakoniwa::PlayerStateFallHakoniwa(
    al::LiveActor* actor, const PlayerConst* playerConst, const IUsePlayerCollision* collider,
    const PlayerInput* input, const PlayerTrigger* trigger, const PlayerAreaChecker* areaChecker,
    const IUseDimension* dimension, PlayerAnimator* animator)
    : al::ActorStateBase("落下", actor), mConst(playerConst), mCollider(collider),
      mTrigger(trigger), mDimension(dimension), mAnimator(animator),
      mJudgeInvalidateInputFall(nullptr), mAirMoveControl(nullptr), mInitialFallMovementCooldown(0),
      mWallCatchCooldown(0), mInitialDirection(0, 0, 0), mWasStart(false), mWasRunning(false),
      mInvalidateInputFallArea(nullptr), mHorizontalSpeedLimit(0) {
    mJudgeInvalidateInputFall = new PlayerJudgeInvalidateInputFall(actor, areaChecker, collider);
    mAirMoveControl = new PlayerActionAirMoveControl(actor, playerConst, input, mCollider, false);
    mAirMoveControl->setupCollideWallScaleVelocity(mConst->getFallWallScaleVelocity(), 0,
                                                   mConst->getNormalMaxSpeed());
    initNerve(&Fall);
}

void PlayerStateFallHakoniwa::appear() {
    al::NerveStateBase::appear();

    mInitialDirection.set(0, 0, 0);
    mWasStart = false;
    mWasRunning = false;
    mInitialFallMovementCooldown = 0;
    mWallCatchCooldown = 0;

    if (mTrigger != nullptr) {
        mWasStart = mTrigger->isOn(PlayerTrigger::ECollisionTrigger_FallOnStart);
        mWasRunning = mTrigger->isOn(PlayerTrigger::EActionTrigger_FallAfterRun);

        if (mTrigger->isOn(PlayerTrigger::EActionTrigger_FallAfterWallCatch)) {
            al::LiveActor* player = mActor;
            mInitialFallMovementCooldown = 8;
            mWallCatchCooldown = 15;
            al::calcFrontDir(&mInitialDirection, player);
        }
    }

    f32 minSpeed = al::calcSpeedH(mActor);
    f32 maxNormalSpeed = mConst->getNormalMaxSpeed();
    f32 maxJumpMoveSpeed = mConst->getJumpMoveSpeedMax();
    if (minSpeed < maxNormalSpeed)
        minSpeed = maxNormalSpeed;
    else if (minSpeed > maxJumpMoveSpeed)
        minSpeed = maxJumpMoveSpeed;
    mHorizontalSpeedLimit = minSpeed;
    mInvalidateInputFallArea = nullptr;
    updateFallTargetArea();
    mAirMoveControl->setPlayer2D(rs::isPlayer2D(mActor));
    al::setNerve(this, &Fall);
}

void PlayerStateFallHakoniwa::updateFallTargetArea() {
    const al::AreaObj* area = mInvalidateInputFallArea;
    if (area == nullptr) {
        if (!rs::updateJudgeAndResult(mJudgeInvalidateInputFall))
            return;

        mInvalidateInputFallArea = area = mJudgeInvalidateInputFall->getArea();
    }

    al::LiveActor* player = mActor;
    const sead::Matrix34f& baseMtx = al::getAreaObjBaseMtx(area);
    sead::Vector3f areaUp = baseMtx.getBase(1);
    sead::Vector3f areaTrans = baseMtx.getBase(3);

    sead::Vector3f hVecToArea = sead::Vector3f(0, 0, 0);
    hVecToArea.setSub(areaTrans, al::getTrans(player));
    al::verticalizeVec(&hVecToArea, areaUp, hVecToArea);

    f32 hDistToArea = hVecToArea.length();

    sead::Vector3f hVelocity = sead::Vector3f(0, 0, 0);
    sead::Vector3f vVelocity = sead::Vector3f(0, 0, 0);
    al::separateVectorHV(&hVelocity, &vVelocity, areaUp, al::getVelocity(player));
    sead::Vector3f hDirToArea = sead::Vector3f(0, 0, 0);

    if (al::tryNormalizeOrZero(&hDirToArea, hVecToArea)) {
        sead::Vector3f parallelVelocity = sead::Vector3f(0, 0, 0);
        sead::Vector3f perpendicularVelocity = sead::Vector3f(0, 0, 0);
        al::separateVectorParallelVertical(&parallelVelocity, &perpendicularVelocity, hDirToArea,
                                           hVelocity);

        // slow down perpendicular speed
        perpendicularVelocity *= 0.75f;

        f32 speedTowardsArea = parallelVelocity.dot(hDirToArea);

        // if speed towards area would overshoot the centre of the area, slow down
        // if speed is facing away from area, slow down
        if (hDistToArea < speedTowardsArea * 5.0f || speedTowardsArea < 0.0f)
            speedTowardsArea *= 0.75f;

        else if (speedTowardsArea < 0.0f)
            speedTowardsArea *= 0.75f;

        speedTowardsArea += 0.1f;

        // cap actual speed to speed limit
        f32 hSpeedToArea = sead::Mathf::min3(hDistToArea, mHorizontalSpeedLimit, speedTowardsArea);

        hVelocity = perpendicularVelocity + hDirToArea * hSpeedToArea;
    } else {
        hVelocity.set(0, 0, 0);
    }

    al::setVelocity(mActor, hVelocity + vVelocity);
}

bool PlayerStateFallHakoniwa::isValidWallCatch() const {
    if (isDead() || mWallCatchCooldown == 0 || al::isGreaterStep(this, mWallCatchCooldown))
        return true;

    sead::Vector3f front = sead::Vector3f(0, 0, 0);
    al::calcFrontDir(&front, mActor);

    return front.dot(mInitialDirection) < 0.70710677f;  // cos(45°)
}

void PlayerStateFallHakoniwa::exeFall() {
    updateFallTargetArea();

    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Fall");
        mAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getJumpMoveSpeedMin(), 0,
                               al::calcSpeedV(mActor), mConst->getJumpGravity(),
                               mInitialFallMovementCooldown, 0);
    }

    if (rs::isPlayer2D(mActor) && rs::isIn2DArea(mDimension)) {
        sead::Vector3f lockDirection = sead::Vector3f(0, 0, 0);
        rs::calcLockDirection(&lockDirection, mDimension);
        mAirMoveControl->verticalizeStartMoveDir(lockDirection);
    }

    mAirMoveControl->update();
}
