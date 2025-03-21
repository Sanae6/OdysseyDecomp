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

class StageSwitchKeeper {
public:
    StageSwitchKeeper();
    void init(StageSwitchDirector*, const PlacementInfo&);
    StageSwitchAccesser* tryGetStageSwitchAccesser(const char* name);

    void setUseName(IUseName* useName) {
        mUseName = useName;
    }

private:
    StageSwitchAccesser* mStageSwitchAccessers;
    s32 mAccesserNum;
    IUseName* mUseName;
};

}  // namespace al
