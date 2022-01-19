#pragma once

#include "al/nerve/HostStateBase.h"
#include "al/util/NerveUtil.h"
#include "game/objects/Yukimaru/YukimaruInput.h"
#include "game/objects/Yukimaru/YukimaruStateMove.h"

namespace al {
class HitSensor;
class SensorMsg;
}

class Yukimaru;

class YukimaruStateHack : public al::HostStateBase<Yukimaru>, public YukimaruInput {
public:
    YukimaruStateHack(Yukimaru*);
    ~YukimaruStateHack();
    void start(al::HitSensor*, al::HitSensor*, bool);
    void tryStartDirect(const al::SensorMsg*);
    void onHackShadowAndSilhouette();
    void appear() override;
    void kill() override;
    void offHackShadowAndSilhouette();
    void control() override;
    void attackSensor(al::HitSensor*, al::HitSensor*);
    void receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool isEndCancel();
    bool isEndDamage();
    bool isEndKill();
    void updateMoveNoInput();
    void updateScale();
    void resetMoveEffect();
    bool isHoldJump() const override;
    bool isTriggerJump() const override;
    void calcInputVec(sead::Vector3f *input) const override;

    void exeWaitEnterMario();
    void exeHackStart();
    void exeMove();
    void exeCancel();
    void exeDamage();
    void exeKill();
    void exeTalkDemo();
};

namespace {
NERVE_HEADER(YukimaruStateHack, WaitEnterMario)
NERVE_HEADER(YukimaruStateHack, HackStart)
NERVE_HEADER(YukimaruStateHack, Move)
NERVE_HEADER(YukimaruStateHack, Cancel)
NERVE_HEADER(YukimaruStateHack, Damage)
NERVE_HEADER(YukimaruStateHack, Kill)
NERVE_HEADER(YukimaruStateHack, TalkDemo)
}  // namespace