#pragma once

namespace al {
class StageSwitchAccesser;
}

namespace al {
class StageSwitchListener;
}

namespace al {

class StageSwitchWatcher {
public:
    StageSwitchWatcher(StageSwitchListener* switchListener, StageSwitchAccesser* switchAccesser);
    void update();

private:
    StageSwitchListener* mSwitchListener;
    StageSwitchAccesser* mSwitchAccesser;
    bool mWasSwitchOn = false;
};

}  // namespace al