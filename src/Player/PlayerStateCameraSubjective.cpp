#include "Player/PlayerStateCameraSubjective.h"

#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"  // IWYU pragma: keep
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/PostProcessing/CameraBlurController.h"
#include "Library/Se/SeFunction.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(PlayerStateCameraSubjective, Start);
NERVE_IMPL(PlayerStateCameraSubjective, Wait);
NERVE_IMPL(PlayerStateCameraSubjective, End);

NERVES_MAKE_NOSTRUCT(PlayerStateCameraSubjective, Start);
NERVES_MAKE_NOSTRUCT(PlayerStateCameraSubjective, Wait);
NERVES_MAKE_NOSTRUCT(PlayerStateCameraSubjective, End);
}  // namespace

PlayerStateCameraSubjective::PlayerStateCameraSubjective(
    al::LiveActor* player, al::CameraTicket* cameraTicket, IPlayerModelChanger* modelChanger,
    PlayerCarryKeeper* carryKeeper, PlayerAnimator* animator, const PlayerConst* playerConst,
    const IUsePlayerCollision* collider, const PlayerInput* input)
    : al::ActorStateBase("主観カメラ", player), mConst(playerConst), mCollider(collider),
      mInput(input), mTurnRate(0), mIsInterruptByDemo(false), mTicket(cameraTicket),
      mModelChanger(modelChanger), mCarryKeeper(carryKeeper), mAnimator(animator) {
    al::setCameraInterpoleStep(cameraTicket, 10);
    initNerve(&Wait);
}

inline static void closeTutorialAndEndCamera(al::LiveActor* actor, al::CameraTicket* ticket) {
    rs::tryClosePlayerCameraSubjectiveTutorial(actor);
    al::endCamera(actor, ticket, 10, false);
}

void PlayerStateCameraSubjective::appear() {
    al::NerveStateBase::appear();
    mIsInterruptByDemo = false;
    alSeFunction::startListenerPoser(mActor, "カメラ位置", 0x1e);
    al::setNerve(this, &Start);
}

void PlayerStateCameraSubjective::kill() {
    if (al::isActiveCamera(mTicket))
        closeTutorialAndEndCamera(mActor, mTicket);

    if (!mIsInterruptByDemo)
        mModelChanger->showModel();

    if (mCarryKeeper->isCarry())
        mCarryKeeper->endCameraSubjective();

    alSeFunction::endListenerPoser(mActor, "カメラ位置", 0);

    al::NerveStateBase::kill();
}

void PlayerStateCameraSubjective::control() {
    rs::waitGround(mActor, mCollider, mConst->getGravity(), mConst->getFallSpeedMax(),
                   mConst->getSlerpQuatRate(), mConst->getWaitPoseDegreeMax());
}

void PlayerStateCameraSubjective::interruptByDemo() {
    if (!al::isActiveCamera(mTicket))
        return;

    closeTutorialAndEndCamera(mActor, mTicket);
    mAnimator->resetModelAlpha();
    mModelChanger->showModel();
    mIsInterruptByDemo = true;
}

bool PlayerStateCameraSubjective::isStateActiveCamera() const {
    return !isDead() && al::isNerve(this, &Wait);
}

void PlayerStateCameraSubjective::exeStart() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::startCamera(actor, mTicket);
        mTurnRate = 0;
        alCameraBlurFunction::setCameraBlurName(actor, "Subjective");
        al::startHitReaction(actor, "主観カメラ起動");
    }

    faceToCameraDir();

    if (!al::isActiveCameraInterpole(actor, 0))
        al::setNerve(this, &Wait);
}

void PlayerStateCameraSubjective::faceToCameraDir() {
    mTurnRate = al::converge(mTurnRate, 15, 1);

    al::LiveActor* actor = mActor;

    sead::Vector3f groundNormal = sead::Vector3f(0, 0, 0);
    rs::calcGroundNormalOrUpDir(&groundNormal, mActor, mCollider);

    sead::Vector3f up = sead::Vector3f(0, 0, 0);
    if (rs::isPlayerInWater(actor))
        up.set(-al::getGravity(actor));
    else
        up.set(groundNormal);

    sead::Vector3f lookDir = sead::Vector3f(0, 0, 0);
    al::calcCameraLookDir(&lookDir, actor, 0);
    al::verticalizeVec(&lookDir, up, lookDir);

    if (al::tryNormalizeOrZero(&lookDir)) {
        f32 turnRate = std::min(mTurnRate / 15.0f, 1.0f);
        sead::Vector3f front = sead::Vector3f(0, 0, 0);
        al::calcFrontDir(&front, actor);
        al::turnVecToVecRate(&front, front, lookDir, turnRate);
        rs::slerpUpFront(actor, up, front, mConst->getSlerpQuatRate(),
                         mConst->getWaitPoseDegreeMax());
    }

    sead::Vector3f normalUp = groundNormal;
    al::setVelocity(actor, -(normalUp * mConst->getGravity()));
}

void PlayerStateCameraSubjective::exeWait() {
    if (al::isFirstStep(this)) {
        mModelChanger->hideModel();
        if (mCarryKeeper->isCarry())
            mCarryKeeper->endCameraSubjective();
        rs::tryAppearPlayerCameraSubjectiveTutorial(mActor);
        mAnimator->startAnim("Wait");
    }

    faceToCameraDir();

    if (mInput->isTriggerCameraSubjective())
        al::setNerve(this, &End);
}

void PlayerStateCameraSubjective::exeEnd() {
    if (al::isFirstStep(this)) {
        al::LiveActor* actor = mActor;
        sead::Vector3f lookDir = sead::Vector3f(0, 0, 0);
        al::calcCameraLookDir(&lookDir, actor, 0);
        al::verticalizeVec(&lookDir, al::getGravity(actor), lookDir);
        if (al::tryNormalizeOrZero(&lookDir))
            al::faceToDirection(actor, lookDir);

        closeTutorialAndEndCamera(mActor, mTicket);
        al::startHitReaction(actor, "主観カメラ終了");
    }

    if (!al::isLessStep(this, 10)) {
        alCameraBlurFunction::resetCameraBlurName(mActor);
        kill();
    }
}
