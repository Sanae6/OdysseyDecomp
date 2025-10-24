#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
}
class IPlayerModelChanger;
class PlayerCarryKeeper;
class PlayerAnimator;
class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;

class PlayerStateCameraSubjective : public al::ActorStateBase {
public:
    PlayerStateCameraSubjective(al::LiveActor* player, al::CameraTicket* cameraTicket,
                                IPlayerModelChanger* modelChanger, PlayerCarryKeeper* carrykeeper,
                                PlayerAnimator* animator, const PlayerConst* playerConst,
                                const IUsePlayerCollision* collider, const PlayerInput* input);
    ~PlayerStateCameraSubjective() = default;

    void appear() override;
    void kill() override;
    void control() override;

    void interruptByDemo();
    bool isStateActiveCamera() const;

    void exeStart();
    void faceToCameraDir();
    void exeWait();
    void exeEnd();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    const PlayerInput* mInput;
    s32 mTurnRate;
    bool mIsInterruptByDemo;
    al::CameraTicket* mTicket;
    IPlayerModelChanger* mModelChanger;
    PlayerCarryKeeper* mCarryKeeper;
    PlayerAnimator* mAnimator;
};
