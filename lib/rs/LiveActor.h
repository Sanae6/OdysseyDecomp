#pragma once

namespace al {
class LiveActor;
}
class CapTargetInfo;

namespace rs {

CapTargetInfo* createCapTargetInfo(al::LiveActor* actor, const char*);

}  // namespace rs
