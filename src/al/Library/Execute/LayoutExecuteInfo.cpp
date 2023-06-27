#include <al/Library/Execute/LayoutExecuteInfo.h>

namespace al {
LayoutExecuteInfo::LayoutExecuteInfo() {}

void LayoutExecuteInfo::addUpdater(al::ExecutorListLayoutUpdate* updater) {
    mUpdaters[mUpdaterCount] = updater;
    mUpdaterCount++;
}
void LayoutExecuteInfo::addDrawer(al::ExecutorListLayoutDrawBase* drawer) {
    mDrawers[mDrawerCount] = drawer;
    mDrawerCount++;
}
}  // namespace al