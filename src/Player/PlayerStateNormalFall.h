#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;
class PlayerAnimator;
class PlayerActionAirMoveControl;

class PlayerStateNormalFall : public al::ActorStateBase {
public:
    PlayerStateNormalFall(al::LiveActor* player, const PlayerConst* playerConst,
                          const IUsePlayerCollision* collider, const PlayerInput* input,
                          PlayerAnimator* animator);
                          ~PlayerStateNormalFall() = default;

    void appear() override;

    void exeFall();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    PlayerAnimator* mAnimator;
    PlayerActionAirMoveControl* mAirMoveControl;
};
