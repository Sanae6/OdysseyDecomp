#include <al/Library/Clipping/ViewIdHolder.h>
#include <al/Library/Placement/PlacementFunction.h>
#include <al/Library/Placement/PlacementInfo.h>

namespace al {
ViewIdHolder::ViewIdHolder() {}
ViewIdHolder* ViewIdHolder::tryCreate(const al::PlacementInfo& placementInfo) {
    if (al::calcLinkChildNum(placementInfo, "ViewGroup") < 1) {
        return nullptr;
    } else {
        ViewIdHolder* holder = new ViewIdHolder();
        holder->init(placementInfo);
        return holder;
    }
}
void ViewIdHolder::init(const al::PlacementInfo& placementInfo) {
    mNumPlacements = al::calcLinkChildNum(placementInfo, "ViewGroup");
    mPlacementIds = new al::PlacementId[mNumPlacements];
    if(mNumPlacements >= 1) {
        for (int i = 0; i < mNumPlacements; ++i) {
            al::PlacementInfo info;
            al::getLinksInfoByIndex(&info, placementInfo, "ViewGroup", i);
            mPlacementIds[i].init(info);
        }
    }
}
al::PlacementId& ViewIdHolder::getViewId(int idx) const {
    return mPlacementIds[idx];
}
}
