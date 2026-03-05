#pragma once

#include <math/seadMatrix.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
class PlacementInfo;
class DemoActor;

class DesignActorInfo {
    DesignActorInfo(al::LiveActor*, const al::ActorInitInfo&, const al::ActorInitInfo&,
                    const sead::Matrix34f&);
};

class DemoActorHolder {
public:
    DemoActorHolder(bool forDemoScene);
    void initPlacementDemoActor(const al::PlacementInfo& placementInfo,
                                const al::ActorInitInfo& initInfo, const sead::Matrix34f& matrix);
    void initPlacementDesignActor(const al::PlacementInfo& placementInfo,
                                  const al::ActorInitInfo& initInfo, const sead::Matrix34f& matrix);
    void appear();
    void kill();
    void setEndCameraInterpolateFrame(bool);
    void startAction(s32);
    void startSequence();
    void updateSequence();
    void updateGraphics();
    bool isActionEndCamera(s32) const;
    bool isEndSequence() const;
    s32 getDemoCameraNum() const;
    bool isExistCamera() const;
    DemoActor* tryFindDemoActor(const char* name);
    const char* getCurrentDemoActionName() const;
    s32 getCurrentDemoFrame() const;
    s32 getCurrentDemoFrameMax() const;
    DemoActor* getDemoActor(s32) const;

private:
};

namespace alDemoFunction {
DemoActorHolder* createDemoActorHolder(const char* stageName, const al::ActorInitInfo& initInfo,
                                       const sead::Matrix34f* matrix, s32 scenarioNo,
                                       bool forDemoScene);
DemoActorHolder* createDemoActorHolderForDemoScene(const char* stageName,
                                                   const al::ActorInitInfo& initInfo,
                                                   const sead::Matrix34f* matrix, s32 scenarioNo);
}  // namespace alDemoFunction
}  // namespace al
