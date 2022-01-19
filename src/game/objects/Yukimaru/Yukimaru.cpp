#include "game/objects/Yukimaru/Yukimaru.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/OtherUtil.h"
#include "rs/LiveActor.h"

Yukimaru::Yukimaru(const char* name) : al::LiveActor(name) {}

void Yukimaru::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "SnowManRacer", "Hack");
    al::setQuat(this, al::getQuat(this)); // todo: how do you actually do this? is this right? probably not...
    al::initJointControllerKeeper(this, 1);
    al::initJointGlobalQuatController(this, al::getQuatPtr(this), "Rotate");
    al::createAndSetColliderSpecialPurpose(this, "MoveLimit");
    rs::createCapTargetInfo(this, "");
    //instantiate
}

void Yukimaru::exeEvent() {}

namespace {
NERVE_IMPL(Yukimaru, Event)
NERVE_IMPL(Yukimaru, Reaction)
NERVE_IMPL(Yukimaru, Hack)
NERVE_IMPL(Yukimaru, HackAfter)
}
