#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class StageInfo;
class DemoActorHolder;
class LayoutTextureRenderer;
class DemoSyncedEventKeeper;
class WipeHolder;
}  // namespace al
class DemoSceneStateSkipDemo;
class SceneAudioSystemPauseController;
class DemoSoundSynchronizer;
class GameDataHolder;

class DemoScene : public al::Scene {
public:
    DemoScene(al::WipeHolder* wipeHolder);
    DemoScene() : DemoScene(nullptr) { }
    ~DemoScene() override;

    void init(const al::SceneInitInfo& initInfo) override;
    void initPlacement(const al::ActorInitInfo& initInfo);
    void appear() override;
    void control() override;
    void kill() override;
    void drawMain() const override;

    virtual void exePlay();
    void exeSkipProc();

    bool isLoadEnd() const;
    virtual void initLayout(const al::LayoutInitInfo& initInfo);
    void initPlacementSky(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo);
    void initPlacementDemo(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo);
    void initPlacementObject(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo, const char*);

protected:
    sead::FixedSafeString<64> mStageName;
    al::DemoActorHolder* mDemoActorHolder = nullptr;
    DemoSceneStateSkipDemo* mDemoSceneStateSkipDemo = nullptr;
    bool mLoadEnded = false;
    al::LayoutTextureRenderer* mLayoutTextureRenderer = nullptr;
    s32 mScenarioNo = 1;
    al::DemoSyncedEventKeeper* mDemoSyncedEventKeeper = nullptr;
    SceneAudioSystemPauseController* mSceneAudioSystemPauseController = nullptr;
    DemoSoundSynchronizer* mDemoSoundSynchronizer = nullptr;
    const char* mDemoName = nullptr;
    s32 mDemoTimer = 0;
    al::WipeHolder* mWipeHolder = nullptr;
    const char* mDemoEndFadeName = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;
};

static_assert(sizeof(DemoScene) == 0x198, "DemoScene size");
