#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class AreaObj;
}
class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;
class PlayerTrigger;
class PlayerAreaChecker;
class IUseDimension;
class PlayerAnimator;
class PlayerJudgeInvalidateInputFall;
class PlayerActionAirMoveControl;

class PlayerStateFallHakoniwa : public al::ActorStateBase {
public:
    PlayerStateFallHakoniwa(al::LiveActor* actor, const PlayerConst* playerConst,
                            const IUsePlayerCollision* collider, const PlayerInput* input,
                            const PlayerTrigger* trigger, const PlayerAreaChecker* areaChecker,
                            const IUseDimension* dimension, PlayerAnimator* animator);
    ~PlayerStateFallHakoniwa() = default;

    void appear() override;

    void updateFallTargetArea();
    bool isValidWallCatch() const;

    void exeFall();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    const PlayerTrigger* mTrigger;
    const IUseDimension* mDimension;
    PlayerAnimator* mAnimator;
    PlayerJudgeInvalidateInputFall* mJudgeInvalidateInputFall;
    PlayerActionAirMoveControl* mAirMoveControl;
    s32 mInitialFallMovementCooldown;
    s32 mWallCatchCooldown;
    sead::Vector3f mInitialDirection;
    bool mWasStart;
    bool mWasRunning;
    const al::AreaObj* mInvalidateInputFallArea;
    f32 mHorizontalSpeedLimit;
};
