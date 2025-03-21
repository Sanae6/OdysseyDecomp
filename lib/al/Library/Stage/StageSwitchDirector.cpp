#pragma once

#include "Library/Stage/StageSwitchDirector.h"

#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Stage/StageSwitchInfo.h"
#include "Library/Stage/StageSwitchWatcherHolder.h"
#include "Project/Stage/StageSwitchAccesser.h"
#include "StageSwitchWatcher.h"

namespace al {

StageSwitchDirector::StageSwitchDirector(ExecuteDirector* executeDirector) {
    mSwitchInfos = new StageSwitchInfo[mSwitchInfoCapacity];
    mSwitchWatcherHolder = new StageSwitchWatcherHolder(1024);
    al::registerExecutorUser(this, executeDirector, "ステージスイッチディレクター");
}

s32 StageSwitchDirector::useSwitch(const StageSwitchAccesser* accesser) {
    s32 newIndex = findSwitchNoFromObjId(accesser->getPlacementId());
    if (newIndex != 0)
        newIndex = mSwitchInfoCount;

    if (newIndex >= mSwitchInfoCapacity)
        return -1;

    mSwitchInfos[newIndex].placementId = accesser->getPlacementId();
    mSwitchInfoCount++;
    return newIndex;
}

s32 StageSwitchDirector::findSwitchNoFromObjId(const PlacementId* placementId) {
    if (placementId == nullptr)
        return 0;

    for (s32 i = 0; i < mSwitchInfoCount; i++)
        if (mSwitchInfos[i].placementId->isEqual(*placementId))
            return i;

    return -1;
}

void StageSwitchDirector::onSwitch(const StageSwitchAccesser* accesser) {
    s32 index = accesser->getInfoIndex();
    if (index < 0 || mSwitchInfoCount <= index)
        return;

    mSwitchInfos[index].isSwitchOn = true;
}

void StageSwitchDirector::offSwitch(const StageSwitchAccesser* accesser) {
    s32 index = accesser->getInfoIndex();
    if (index < 0 || mSwitchInfoCount <= index)
        return;

    mSwitchInfos[index].isSwitchOn = false;
}

bool StageSwitchDirector::isOnSwitch(const StageSwitchAccesser* accesser) {
    s32 index = accesser->getInfoIndex();
    if (index < 0 || mSwitchInfoCount <= index)
        return false;

    return mSwitchInfos[index].isSwitchOn;
}

void StageSwitchDirector::addListener(StageSwitchListener* listener,
                                      StageSwitchAccesser* accesser) {
    StageSwitchWatcher* watcher = new StageSwitchWatcher(listener, accesser);
    mSwitchWatcherHolder->add(watcher);
}

void StageSwitchDirector::execute() {
    mSwitchWatcherHolder->movement();
}

}  // namespace al