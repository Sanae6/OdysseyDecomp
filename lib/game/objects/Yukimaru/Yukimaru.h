#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/util/NerveUtil.h"
#include "game/cap/CapTargetInfo.h"
#include "al/event/EventFlowEventData.h"
#include "al/event/IEventFlowEventReceiver.h"

// TODO: Clean this up
namespace al {
class EventFlowEventData;
}  // namespace al

class Yukimaru : public al::LiveActor, al::IEventFlowEventReceiver {
public:
    Yukimaru(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void movement() override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;
    bool isEnableHack();
    void afterHack();
    bool isEnableReaction();
    void receiveEvent(const al::EventFlowEventData*) override;
    void disableHack();
    void enableHack();
    void exeEvent();
    void exeReaction();
    void exeHack();
    void exeHackAfter();

private:
    CapTargetInfo* mCapTargetInfo;
};

namespace {
NERVE_HEADER(Yukimaru, Event)
NERVE_HEADER(Yukimaru, Reaction)
NERVE_HEADER(Yukimaru, Hack)
NERVE_HEADER(Yukimaru, HackAfter)
}  // namespace