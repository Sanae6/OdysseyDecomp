#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"
#include "Player/PlayerPushReceiver.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {
class LiveActor;
class HitSensor;
}  // namespace al
class IUsePlayerCollision;
class PlayerAnimator;
class IPlayerModelChanger;
class IUsePlayerCeilingCheck;
class PlayerJointParamHandLegAngle;

class PlayerCarryKeeper : public al::NerveExecutor {
public:
    PlayerCarryKeeper(const al::LiveActor* player, al::HitSensor* carrySensor,
                      PlayerAnimator* animator, const IPlayerModelChanger* modelChanger,
                      const IUsePlayerCeilingCheck* ceilingCheck,
                      PlayerJointParamHandLegAngle* handLegAngleParam);
    ~PlayerCarryKeeper() = default;

    void update();
    bool updateCollideLockUp(const IUsePlayerCollision* collider, PlayerPushReceiver* pushReceiver);
    void startCarry(al::HitSensor* heldSensor);
    void startThrow(bool swing);
    void startCancelAndRelease();
    void startRelease();
    void startReleaseDemo();
    void startReleaseDamage();
    void startReleaseDead();
    void startDemoKeepCarry();
    void startDemoShineGet();
    void endDemoShineGet();
    const char* getCarryStartAnimName() const;
    void startCameraSubjective();
    void endCameraSubjective();
    void sendPlayerWarp();
    bool isCarry() const;
    bool isCarryWallKeep() const;
    bool isThrowHandR() const;
    const char* getCarryThrowAnimName() const;
    bool isThrowHold() const;
    s32 getThrowReleaseFrame() const;
    bool isThrowRelease() const;
    void exeWait();
    void exeStart();
    void updateHandJointAngle();
    void exeCarry();
    const char* getCarryAnimName() const;
    void exeThrow();
    void exeRelease();

private:
    al::LiveActor* mPlayer;
    PlayerAnimator* mAnimator;
    al::HitSensor* mCarrySensor;
    al::HitSensor* mHeldSensor;
    IPlayerModelChanger* mModelChanger;
    IUsePlayerCeilingCheck* mCeilingCheck;
    PlayerJointParamHandLegAngle* mHandLegAngleParam;
    s32 _48;
    bool mIsCarryAbove;
    bool mIsCarryWallKeep;
    sead::Vector3f _50;
    s32 _5c;
    sead::Vector3f _60;
};
