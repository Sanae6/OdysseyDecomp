#pragma once

#include "Library/Stage/StageSwitchKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/HostIO/IUseName.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Stage/StageSwitchAccesser.h"

namespace al {
StageSwitchAccesserList::StageSwitchAccesserList() {}

StageSwitchAccesserList::StageSwitchAccesserList(const StageSwitchAccesser* accessers)
    : mStageSwitchAccessers(accessers) {}

StageSwitchKeeper::StageSwitchKeeper() {}

void StageSwitchKeeper::init(StageSwitchDirector* director, const PlacementInfo& placementInfo) {
    s32 stageSwitchCount = al::calcLinkCountClassName(placementInfo, "StageSwitch");
    mList.mAccesserCapacity = stageSwitchCount;
    mList.mStageSwitchAccessers = new StageSwitchAccesser[mList.mAccesserCapacity];

    al::PlacementInfo links;
    al::tryGetPlacementInfoByKey(&links, placementInfo, "Links");

    s32 linkCount = al::getCountPlacementInfo(placementInfo);
    for (s32 i = 0; i < linkCount; i++) {
        al::PlacementInfo link;
        const char* linkName;
        al::tryGetPlacementInfoAndKeyNameByIndex(&link, &linkName, links, i);
        if (al::isClassName(link, "StageSwitch")) {
            al::PlacementId placementId;
            al::tryGetPlacementId(&placementId, link);
            auto accesser = mList.mStageSwitchAccessers[i];
            accesser.setUseName(mUseName);
            accesser.init(director, linkName, placementId);
        }
    }
}

const StageSwitchAccesser* StageSwitchKeeper::tryGetStageSwitchAccesser(const char* linkName) const {
    for (s32 i = 0; i < mList.mAccesserCapacity; i++)
        if (al::isEqualString(linkName, mList.mStageSwitchAccessers[i].getLinkName()))
            return &mList.mStageSwitchAccessers[i];

    return nullptr;
}
}  // namespace al
