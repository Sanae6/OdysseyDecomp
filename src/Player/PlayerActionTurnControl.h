#pragma once

#include <container/seadRingBuffer.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerActionTurnControl {
public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(f32 mTurnAngleStart, f32 turnAngleFast, f32 turnAngleLimit, f32 turnAngleFastLimit,
               s32 turnAccelFrame, s32 turnAccelFrameFast, s32 turnBrakeFrame);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, f32);
    f32 calcTurnPowerRate(const sead::Vector3f&) const;

    const sead::Vector3f& get_5c() { return mFront; }

    bool get_69() const { return _69; }

    void set_88(bool value) { _88 = value; }
    void set_89(bool value) { _89 = value; }

private:
    al::LiveActor* mActor;  // used in many non-player contexts
    f32 mTurnAngleTime = 0.0f;
    sead::Vector3f mPreviousMoveVec = sead::Vector3f(0, 0, 0);
    sead::Vector3f mDesiredDirection = sead::Vector3f(0, 0, 0);
    sead::Vector3f mPreviousGroundNormal = sead::Vector3f(0, 0, 0);
    f32 _30 = 0.0f;
    sead::RingBuffer<f32> mRingBuffer;
    sead::Vector3f mFrontAlongNormal = sead::Vector3f(0, 0, 0);
    sead::Vector3f mFront = sead::Vector3f(0, 0, 0);
    bool _68 = false;
    bool _69 = false;
    bool _6a = false;
    f32 mTurnAngleStart = 0.0f;
    f32 mTurnAngleFast = 0.0f;
    f32 mTurnAngleLimit = 0.0f;
    f32 mTurnAngleFastLimit = 0.0f;
    s32 mTurnAccelFrame = 0;
    s32 mTurnAccelFrameFast = 0;
    s32 mTurnBrakeFrame = 0;
    bool _88 = false;
    bool _89 = false;
};

static_assert(sizeof(PlayerActionTurnControl) == 0x90);
