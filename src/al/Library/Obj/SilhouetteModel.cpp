#include <al/Library/LiveActor/ActorInitInfo.h>
#include <al/Library/Obj/SilhouetteModel.h>

namespace al {
SilhouetteModel::SilhouetteModel(al::LiveActor* parent,const al::ActorInitInfo& initInfo, char const* category) : al::LiveActor("シルエットモデル") {
    al::initActorSceneInfo(this, initInfo);
    initPoseKeeper(parent->getPoseKeeper());
    al::initActorModelKeeperByHost(this, parent);
    al::initActorClipping(this, initInfo);
    al::invalidateClipping(this);
    al::initExecutorDraw(this, initInfo, category);
    makeActorAlive();
}
void SilhouetteModel::movement() {}
}