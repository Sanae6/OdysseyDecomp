#pragma once

#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class WipeHolder;
class ScreenCaptureExecutor;
}  // namespace al
class HakoniwaSequence;
class HakoniwaStateDeleteScene;
class DemoScene;

class HakoniwaStateSimpleDemo : al::HostStateBase<HakoniwaSequence> {
public:
    HakoniwaStateSimpleDemo(HakoniwaSequence* sequence, al::WipeHolder* holder,
                            al::ScreenCaptureExecutor* executor,
                            HakoniwaStateDeleteScene* deleteSceneState);

    void init() override;
    void appear() override;

    void setStageName(const char* stageName);
    void setWipeName(const char* wipeName);

    void exeLoad();
    void exeDemo();
    void exeEnd();

private:
    DemoScene* mDemoScene;
    al::WipeHolder* mWipeHolder;
    al::ScreenCaptureExecutor* mScreenCaptureExecutor;
    sead::FixedSafeString<128> mStageName;
    sead::FixedSafeString<128> mWipeName;
    HakoniwaStateDeleteScene* mDeleteSceneState;
};

static_assert(sizeof(HakoniwaStateSimpleDemo) == 0x170);
