#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Stage/StageSwitchKeeper.h"

#include "IUseStageSwitch.h"
#include "Library/Placement/PlacementFunction.h"
#include "Project/Stage/StageSwitchAccesser.h"

namespace al {
void initStageSwitch(IUseStageSwitch* user, StageSwitchDirector* stageSwitchDirector,
                     const PlacementInfo& placementInfo) {
    if (user->getStageSwitchKeeper() != nullptr)
        return;

    user->initStageSwitchKeeper();
    s32 linkCount = al::calcLinkCountClassName(placementInfo, "StageSwitch");
    if (linkCount != 0) {
        StageSwitchKeeper* keeper = user->getStageSwitchKeeper();
        keeper->setUseName(user);
        keeper->init(stageSwitchDirector, placementInfo);
    }
}

bool isValidStageSwitch(const IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    return accesser->isValid();
}

bool isOnStageSwitch(const IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    return accesser->isOnSwitch();
}

void onStageSwitch(IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return;
    auto accesser = const_cast<StageSwitchAccesser*>(keeper->tryGetStageSwitchAccesser(linkName));
    if (accesser == nullptr)
        return;

    accesser->isEnableRead();
    accesser->onSwitch();
}

void offStageSwitch(IUseStageSwitch* user, const char* linkName) {
    StageSwitchKeeper* keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return;
    StageSwitchAccesser* accesser = const_cast<StageSwitchAccesser*>(keeper->tryGetStageSwitchAccesser(linkName));
    if (accesser == nullptr)
        return;

    accesser->isEnableRead();
    accesser->offSwitch();
}

bool tryOnStageSwitch(IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = const_cast<StageSwitchAccesser*>(keeper->tryGetStageSwitchAccesser(linkName));
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    if (!accesser->isValid() || accesser->isOnSwitch())
        return false;
    accesser->onSwitch();
    return true;
}

bool tryOffStageSwitch(IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = const_cast<StageSwitchAccesser*>(keeper->tryGetStageSwitchAccesser(linkName));
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    if (!accesser->isValid() || !accesser->isOnSwitch())
        return false;
    accesser->offSwitch();
    return true;
}

bool isSameStageSwitch(const IUseStageSwitch* user, const IUseStageSwitch* otherUser,
                       const char* linkName) {

    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (accesser == nullptr)
        return false;
    accesser->isEnableRead();

    auto otherKeeper = user->getStageSwitchKeeper();
    if (otherKeeper == nullptr)
        return false;
    auto otherAccesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (otherAccesser == nullptr)
        return false;
    otherAccesser->isEnableRead();
    return accesser->isEqualSwitch(otherAccesser);
}

bool isValidSwitchAppear(const IUseStageSwitch* user) {

    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = keeper->tryGetStageSwitchAccesser("SwitchAppear");
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    return accesser->isValid();
}

bool isOnStageSwitch(IUseStageSwitch* user, const char* linkName) {
    auto keeper = user->getStageSwitchKeeper();
    if (keeper == nullptr)
        return false;
    auto accesser = keeper->tryGetStageSwitchAccesser(linkName);
    if (accesser == nullptr)
        return false;

    accesser->isEnableRead();
    return accesser->isOnSwitch();
}

bool isOnSwitchAppear(const IUseStageSwitch* user);
bool isValidSwitchKill(const IUseStageSwitch* user);
bool isValidSwitchDeadOn(const IUseStageSwitch* user);
bool tryOnSwitchDeadOn(IUseStageSwitch* user);
bool tryOffSwitchDeadOn(IUseStageSwitch* user);
void onSwitchDeadOn(IUseStageSwitch* user);
void offSwitchDeadOn(IUseStageSwitch* user);
bool isOnSwitchDeadOn(IUseStageSwitch* user);
bool isOffSwitchDeadOn(IUseStageSwitch* user);
bool isValidSwitchStart(const IUseStageSwitch* user);
bool isOnSwitchStart(const IUseStageSwitch* user);
bool listenStageSwitchOff(IUseStageSwitch* user, const char* eventName,
                          const al::FunctorBase& actionOnOff);
bool listenStageSwitchOn(IUseStageSwitch* user, const char* eventName,
                         const FunctorBase& actionOnOn);
bool listenStageSwitchOnAppear(IUseStageSwitch* user, const FunctorBase& actionOnAppear);
bool listenStageSwitchOnOff(IUseStageSwitch* user, const char* eventName,
                            const FunctorBase& actionOnOn, const FunctorBase& actionOnOff);
bool listenStageSwitchOnOffStart(IUseStageSwitch* user, const FunctorBase& actionOnOn,
                                 const FunctorBase& actionOnOff);
bool listenStageSwitchOnOffAppear(IUseStageSwitch* user, const FunctorBase& actionOnOn,
                                  const FunctorBase& actionOnOff);
bool listenStageSwitchOnKill(IUseStageSwitch* user, const FunctorBase& actionOnOn);
bool listenStageSwitchOnOffKill(IUseStageSwitch* user, const FunctorBase& actionOnOn,
                                const FunctorBase& actionOnOff);
bool listenStageSwitchOnStart(IUseStageSwitch* user, const FunctorBase& actionOnOn);
bool listenStageSwitchOnStop(IUseStageSwitch* user, const FunctorBase& actionOnOn);

}  // namespace al
