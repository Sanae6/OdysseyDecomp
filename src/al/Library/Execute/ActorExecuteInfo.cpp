#include <al/Library/Execute/ActorExecuteInfo.h>

namespace al {
ActorExecuteInfo::ActorExecuteInfo(al::ExecuteRequestKeeper* keeper) {
    mRequestKeeper = keeper;
}
void ActorExecuteInfo::addUpdater(al::ExecutorActorExecuteBase* updater) {
    mUpdaters[mUpdaterCount] = updater;
    mUpdaterCount++;
}
void ActorExecuteInfo::addDrawer(al::ModelDrawerBase* drawer) {
    mDrawers[mDrawerCount] = drawer;
    mDrawerCount++;
}
}  // namespace al