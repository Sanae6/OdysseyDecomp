#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerActionTurnControl;

class PlayerActionAirMoveControl {
public:
    PlayerActionAirMoveControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                               const IUsePlayerCollision*, bool);
    void setup(f32 maxHorizontalSpeed, f32 maxVerticalSpeed, s32 extendFrames, f32 initialSpeed,
               f32 gravityPower, s32 initialMovementCooldown, f32 inertiaRate);
    void setupTurn(f32, f32, f32, f32, s32, s32, s32);
    void setExtendFrame(s32);
    void setupCollideWallScaleVelocity(f32, f32, f32);
    void verticalizeStartMoveDir(const sead::Vector3f&);
    void update();
    bool isHoldJumpExtend() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&) const;

    void setPlayer2D(bool isPlayer2D) { mIsPlayer2D = isPlayer2D; }

private:
    al::LiveActor* mPlayer;
    PlayerConst* mConst;
    PlayerInput* mInput;
    IUsePlayerCollision* mCollider;
    IUsePlayerHack** mHack;
    PlayerActionTurnControl* mTurnControl;
    bool _30;
    bool mHasExtendFrames;
    bool _32;
    bool mIsPlayer2D;
    s32 mExtendFrames;
    s32 mCurrentExtendFrame;
    s32 mInitialVelocityCooldown;
    bool mUseUpAsGravity;
    bool _41;
    f32 _44;
    f32 _48;
    sead::Vector3f _4c;
    sead::Vector3f _58;
    f32 mVelocityLimit;
    f32 mVerticalSpeed;
    f32 _6c;
    bool _70;

    f32 mWallScaleVelocity;
    f32 _78;
    f32 mWallFallSpeed;
    f32 mSlerpQuatRate;
    sead::Vector3f _84;
};

static_assert(sizeof(PlayerActionAirMoveControl) == 0x90);
