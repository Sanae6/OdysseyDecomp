#include "Player/PlayerActionTurnControl.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Player/PlayerActionFunction.h"
#include "math/seadMathCalcCommon.h"
#include "math/seadMathNumbers.h"
#include "math/seadQuat.h"
#include "math/seadQuatCalcCommon.h"
#include "math/seadVectorCalcCommon.h"
#include "math/seadVectorFwd.h"

PlayerActionTurnControl::PlayerActionTurnControl(al::LiveActor* actor) : mActor(actor) {
    mRingBuffer.allocBuffer(5, nullptr);
}

void PlayerActionTurnControl::setup(f32 turnAngleStart, f32 turnAngleFast, f32 turnAngleLimit,
                                    f32 turnAngleFastLimit, s32 turnAccelFrame,
                                    s32 turnAccelFrameFast, s32 turnBrakeFrame) {
    mTurnAngleStart = turnAngleStart;
    mTurnAngleFast = turnAngleFast;
    mTurnAngleLimit = turnAngleLimit;
    mTurnAngleFastLimit = turnAngleFastLimit;
    mTurnAccelFrame = turnAccelFrame;
    mTurnAccelFrameFast = turnAccelFrameFast;
    mTurnBrakeFrame = turnBrakeFrame;
}

void PlayerActionTurnControl::reset() {
    mRingBuffer.clear();
    _30 = 0.0f;
    mTurnAngleTime = 0.0f;
    mPreviousMoveVec.set(0, 0, 0);
    mDesiredDirection.set(0, 0, 0);
    mPreviousGroundNormal.set(0, 0, 0);
}

void PlayerActionTurnControl::update(const sead::Vector3f& moveVec,
                                     const sead::Vector3f& groundNormal) {
    sead::Vector3f frontHorizontal = sead::Vector3f(0, 0, 0);
    al::calcFrontDir(&frontHorizontal, mActor);
    sead::Vector3f upDir = sead::Vector3f(0, 0, 0);
    al::calcUpDir(&upDir, mActor);
    al::alongVectorNormalH(&frontHorizontal, frontHorizontal, upDir, groundNormal);
    al::tryNormalizeOrZero(&frontHorizontal);
    mFrontAlongNormal.set(frontHorizontal);  // nm: writes each f32, f32, f32 (3x str w) while
                                             // current is writing f32, f32x2 (str w, str x)
    sead::Vector3f previousMoveVecH = sead::Vector3f(0, 0, 0);
    if (!al::isNearZero(mPreviousMoveVec, 0.001) || !al::isNearZero(mPreviousGroundNormal, 0.001)) {
        al::alongVectorNormalH(&previousMoveVecH, mPreviousMoveVec, mPreviousGroundNormal,
                               groundNormal);
        al::tryNormalizeOrZero(&previousMoveVecH);
    }
    sead::Vector3f gravity = al::getGravity(mActor);

    sead::Vector3f moveVecNorm = sead::Vector3f(0, 0, 0);
    bool isMoving = al::tryNormalizeOrZero(&moveVecNorm, moveVec);
    bool bVar6 = false;

    if (upDir.dot(-gravity) < 0.087155744f)  // cos(85deg)
        bVar6 = _89;

    bool isFrontHoriZero = al::isNearZero(frontHorizontal, 0.001f);
    bool something = bVar6 || isFrontHoriZero;
    bool wasNotMoving = al::isNearZero(previousMoveVecH, 0.001f);
    bool isNotMoving = !isMoving;
    bool startedMoving = isMoving && !wasNotMoving;
    f32 fVar20 = 0.0f;
    if (wasNotMoving || isNotMoving || moveVecNorm.dot(previousMoveVecH) < 0.34202015f) {
        mRingBuffer.clear();
        fVar20 = 0.0f;
        _30 = 0.0f;
    } else {
        if (startedMoving == 0)
            fVar20 = 0.0;
        else
            fVar20 = sead::Mathf::abs(al::calcAngleDegree(previousMoveVecH, moveVecNorm));

        _30 = 0.0f;
        // todo: is this the right operation?
        mRingBuffer.forcePushBack(fVar20);

        if (!mRingBuffer.size())
            fVar20 = 0;
        else
            for (f32 value : mRingBuffer)
                _30 = fVar20 += value;
        fVar20 = fVar20 / 5.0;
        _30 = fVar20;
    }
    bool bVar14 = isMoving;
    bool bVar7;
    if (!bVar6 && !isFrontHoriZero && !isNotMoving) {
        bool uVar11 = PlayerActionFunction::isOppositeDir(moveVecNorm, frontHorizontal);
        bVar7 = false;
        if ((!wasNotMoving && !isNotMoving) && !(startedMoving && (fVar20 <= 1.0))) {
            isNotMoving = false;
        } else {
            isNotMoving = false;
            if (uVar11) {
                bVar7 = !_88;
                isNotMoving = !bVar7;
                bVar14 = isMoving && isNotMoving;
            }
        }
    } else {
        bVar7 = false;
        isNotMoving = false;
    }
    _69 = isNotMoving;
    _68 = bVar7;
    _6a = bVar6;
    sead::Vector3f local_e0 = sead::Vector3f(0, 0, 0);

    // control flow sucks ass, not sure how to approach this.
    // every return point will set mFront to l
    // rest of the function feels like an else block where it ends with mFront.set(local_e0)
    if (!(bVar14 && something)) {
        if (isMoving) {
            if (bVar7) {
                local_e0.set(frontHorizontal);
                mTurnAngleTime = 0.0;
                mFront.set(local_e0);
                return;
            }
            local_e0.set(moveVecNorm);
            if (!bVar6) {
                mFront.set(local_e0);
                return;
            }
        } else {
            if (!bVar6) {
                if (!isFrontHoriZero)
                    local_e0.set(frontHorizontal);
                else
                    al::calcFrontDir(&local_e0, mActor);
                mPreviousGroundNormal.set(0, 0, 0);
                mPreviousMoveVec.set(0, 0, 0);
                mDesiredDirection.set(0, 0, 0);
                mTurnAngleTime = PlayerActionFunction::brake(mTurnAngleTime, mTurnBrakeFrame,
                                                             mTurnAngleLimit);

                mFront.set(local_e0);
                return;
            }
            al::calcFrontDir(&local_e0, mActor);
            if (upDir.dot(-gravity) < 0.0)
                local_e0.set(-local_e0);
            mPreviousGroundNormal.set(0, 0, 0);
            mPreviousMoveVec.set(0, 0, 0);
            mDesiredDirection.set(0, 0, 0);
        }
        mTurnAngleTime = mTurnAngleFastLimit;
        mFront.set(local_e0);
        return;
    }

    sead::Vector3f currentOrPrev = !startedMoving ? frontHorizontal : previousMoveVecH;
    f32 betweenDesiredAndCurrentOrPrev =
        sead::Mathf::acos(sead::Mathf::clamp((currentOrPrev).dot(moveVecNorm), -1.0f, 1.0f));

    f32 betweenDesiredAndCurrent =
        sead::Mathf::acos(sead::Mathf::clamp(frontHorizontal.dot(moveVecNorm), -1.0f, 1.0f));
    sead::Vector3f local_f0 = previousMoveVecH.cross(moveVecNorm);
    al::tryNormalizeOrZero(&local_f0);
    if (!startedMoving || al::isNearZero(mDesiredDirection, 0.001)) {
        // we didn't start moving, and we aren't trying to turn anywhere
        bVar6 = false;
        isMoving = false;
    } else {
        f32 degreesBetweenDesiredAndCurrent = sead::Mathf::rad2deg(betweenDesiredAndCurrentOrPrev);
        if (local_f0.dot(mDesiredDirection) <= 0.0) {
            f32 turnAngleStart = mTurnAngleStart;
            f32 turnAngleTime = mTurnAngleTime;
            if (turnAngleStart <= turnAngleTime)
                turnAngleStart = turnAngleTime;
            if (turnAngleStart <= degreesBetweenDesiredAndCurrent) {
                bVar6 = false;
                mTurnAngleTime = 0.0;
            } else {
                turnAngleStart = al::converge(turnAngleTime, 0.0, degreesBetweenDesiredAndCurrent);
                mTurnAngleTime = turnAngleStart;
                bVar6 = degreesBetweenDesiredAndCurrent < mTurnAngleStart;
            }
            isMoving = false;
        } else {
            sead::Vector3f local_110 = frontHorizontal.cross(moveVecNorm);
            bVar6 = al::tryNormalizeOrZero(&local_110);
            if (!bVar6) {
                bVar6 = false;
                isMoving = false;
            } else if (local_110.dot(local_f0) >= 0.0f) {
                isMoving = false;
                bVar6 = false;
            } else {
                isMoving = fVar20 < 10.0f || degreesBetweenDesiredAndCurrent < 3.0f;
                bVar6 = fVar20 <= 10.0f && degreesBetweenDesiredAndCurrent <= 3.0f;
            }
        }
    }
    f32 length = moveVec.length();
    betweenDesiredAndCurrent = sead::Mathf::rad2deg(betweenDesiredAndCurrent);
    f32 fVar28 = al::calcRate01(length, 0.0f, 0.8f);
    length = sead::Mathf::clamp(fVar28, 0.25f, 1.0f);
    fVar28 = mTurnAngleLimit;
    if (mTurnAngleStart <= betweenDesiredAndCurrent) {
        fVar20 = mTurnAngleTime;
        s32 iVar22;
        if (betweenDesiredAndCurrent < mTurnAngleFast) {
            length = mTurnAngleFastLimit;
            iVar22 = mTurnAccelFrameFast;
        } else {
            length = length * fVar28;
            if (fVar20 < length) {
                fVar28 = mTurnAngleFastLimit;
                goto LAB_710041a1c0;
            }
            iVar22 = mTurnAccelFrame;
        }
        mTurnAngleTime = PlayerActionFunction::accel(fVar20, length, length / (f32)iVar22);
    } else {
        fVar20 = mTurnAngleTime;
    LAB_710041a1c0:
        mTurnAngleTime = PlayerActionFunction::brake(fVar20, mTurnBrakeFrame, fVar28);
    }
    sead::Vector3f local_100 = sead::Vector3f(0, 0, 0);
    if (isMoving || al::isReverseDirection(frontHorizontal, moveVecNorm, 0.01f)) {
        sead::Vector3f gaming = groundNormal;
        if (local_f0.dot(groundNormal) <= 0.0)
            gaming = -gaming;

        sead::Quatf quat;
        sead::QuatCalcCommon<f32>::setAxisAngle(quat, local_f0, mTurnAngleTime);
        local_e0.setRotated(quat, frontHorizontal);
        al::tryNormalizeOrZero(&local_e0);
        local_100.set(groundNormal);
    } else {
        sead::Quatf local_110;
        local_110.makeUnit();
        al::makeQuatRotationLimit(&local_110, frontHorizontal, moveVecNorm,
                                  sead::Mathf::deg2rad(mTurnAngleTime));
        local_e0.setRotated(local_110, frontHorizontal);
        al::normalize(&local_e0);
        local_100.setCross(frontHorizontal, moveVecNorm);
        al::tryNormalizeOrZero(&local_100);
        if (bVar6 && local_100.dot(mDesiredDirection) < 0.0)
            local_100.set(-local_100);
    }
    mDesiredDirection.set(local_100);
    mPreviousMoveVec.set(moveVecNorm);
    mPreviousGroundNormal.set(groundNormal);
    mFront.set(local_e0);
    return;
}

void PlayerActionTurnControl::calcTilt(sead::Vector3f* outVec,
                                       const sead::Vector3f& currentDirection, f32 maxTiltDegree) {
    sead::Vector3f front = mFront;
    sead::Quatf quat;
    sead::QuatCalcCommon<f32>::setAxisAngle(quat, front,
                                            calcTurnPowerRate(currentDirection) * maxTiltDegree);
    outVec->setRotated(quat, currentDirection);

    // quat.calcRPY(*outVec);
}

f32 PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& currentDirection) const {
    f32 turnPowerRate =
        al::easeIn(al::calcRate01(mTurnAngleTime, mTurnAngleStart, mTurnAngleLimit));
    return currentDirection.dot(mDesiredDirection) > 0 ? -turnPowerRate : turnPowerRate;
}
