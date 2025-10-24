#include "Player/PlayerStateNormalFall.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateNormalFall, Fall);

NERVES_MAKE_NOSTRUCT(PlayerStateNormalFall, Fall);
}  // namespace

PlayerStateNormalFall::PlayerStateNormalFall(al::LiveActor* player, const PlayerConst* playerConst,
                                             const IUsePlayerCollision* collider,
                                             const PlayerInput* input, PlayerAnimator* animator)
    : al::ActorStateBase("落下", player), mConst(playerConst), mCollider(collider),
      mAnimator(animator), mAirMoveControl(nullptr) {
    if (input != nullptr)
        mAirMoveControl =
            new PlayerActionAirMoveControl(player, playerConst, input, collider, false);

    initNerve(&Fall);
}

void PlayerStateNormalFall::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Fall);
}

void PlayerStateNormalFall::exeFall() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this)) {
        if (mAnimator == nullptr)
            al::startAction(actor, "Fall");
        else
            mAnimator->startAnim("Fall");

        if (mAirMoveControl != nullptr)
            mAirMoveControl->setup(100.0, mConst->getJumpMoveSpeedMin(), 0, 0.0,
                                   mConst->getJumpGravity(), 0, mConst->getJumpInertiaRate());
    }

    if (mAirMoveControl != nullptr)
        mAirMoveControl->update();

    bool onGround =
        mCollider != nullptr ? rs::isOnGround(actor, mCollider) : al::isOnGround(actor, 0);

    if (!onGround)
        return;

    if (mCollider == nullptr)
        al::reboundVelocityFromCollision(actor, 0.0, 0.0, 1.0);
    else
        rs::reboundVelocityFromCollision(actor, mCollider, 0.0, 0.0, 1.0);

    kill();
}
