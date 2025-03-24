#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ExecuteDirector;
class PlacementId;
class StageSwitchAccesser;
class StageSwitchDirector;
class StageSwitchListener;
class PlacementInfo;
class IUseStageSwitch;
class FunctorBase;
class IUseName;

class StageSwitchAccesserList {
    friend class StageSwitchKeeper;
public:
    StageSwitchAccesserList();
    StageSwitchAccesserList(const StageSwitchAccesser* accessers);

private:
    const StageSwitchAccesser* mStageSwitchAccessers = nullptr;
    s32 mAccesserCapacity = 0;
};

class StageSwitchKeeper {
public:
    StageSwitchKeeper();
    void init(StageSwitchDirector* director, const PlacementInfo& placementInfo);
    const StageSwitchAccesser* tryGetStageSwitchAccesser(const char* linkName) const;

    void setUseName(IUseName* useName) {
        mUseName = useName;
    }

private:
    StageSwitchAccesserList mList;
    IUseName* mUseName = nullptr;
};

}  // namespace al
