#pragma once

namespace al {
class LiveActor;
class ActorInitInfo;
}  // namespace al

namespace rs {

void* initEventFlowFromPlacementInfo(al::LiveActor*, al::ActorInitInfo const&, char const*);

}