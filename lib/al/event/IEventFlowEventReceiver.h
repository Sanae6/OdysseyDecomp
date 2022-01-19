#pragma once

namespace al {
class EventFlowEventData;

class IEventFlowEventReceiver {
public:
    virtual void receiveEvent(const al::EventFlowEventData*) = 0;
};
}  // namespace al
