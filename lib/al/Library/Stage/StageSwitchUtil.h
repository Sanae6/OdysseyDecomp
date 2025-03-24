#pragma once

namespace al {
class FunctorBase;
class IUseStageSwitch;
class StageSwitchDirector;
class PlacementInfo;

void initStageSwitch(IUseStageSwitch* user, StageSwitchDirector* stageSwitchDirector,
                     const PlacementInfo& placementInfo);
bool isValidStageSwitch(const IUseStageSwitch* user, const char* linkName);
bool isOnStageSwitch(const IUseStageSwitch* user, const char* linkName);
void onStageSwitch(IUseStageSwitch* user, const char* linkName);
void offStageSwitch(IUseStageSwitch* user, const char* linkName);
bool tryOnStageSwitch(IUseStageSwitch* user, const char* linkName);
bool tryOffStageSwitch(IUseStageSwitch* user, const char* linkName);
bool isSameStageSwitch(const IUseStageSwitch* user, const IUseStageSwitch* otherUser,
                       const char* linkName);
bool isValidSwitchAppear(const IUseStageSwitch* user);
bool isOffStageSwitch(IUseStageSwitch* user, const char* linkName);
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
bool listenStageSwitchOnAppear(IUseStageSwitch* user,
                               const FunctorBase& actionOnAppear);
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
