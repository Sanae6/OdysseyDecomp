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

class WorldWarpHoleScene : public al::Scene {
public:
    WorldWarpHoleScene(al::WipeHolder* wipeHolder);

    WorldWarpHoleScene() : WorldWarpHoleScene(nullptr) {}

    ~WorldWarpHoleScene() override;

    void init(const al::SceneInitInfo& initInfo) override;
    void initPlacement(const al::ActorInitInfo& initInfo);
    void appear() override;
    void control() override;
    void drawMain() const override;

    void exeStart();
    void exeLoop();
    void exeEnd();
    void exeSkipProc();

    bool isEnableLoopEnd() const;
    void tryEnd();

    void initPlacementSky(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo);
    void initPlacementDemo(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo);
    void initPlacementObject(const al::StageInfo* stageInfo, const al::ActorInitInfo& initInfo,
                             const char*);

protected:
    sead::FixedSafeString<64> mStageName;
    s32 mScenarioNo = 1;
    al::DemoActorHolder* mDemoActorHolder = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;
    al::DemoSyncedEventKeeper* mDemoSyncedEventKeeper = nullptr;
    s32 mDemoTimer = 0;
    sead::FixedSafeString<64> mDemoName;
    s32 mLoopTimer = 0;
    DemoSceneStateSkipDemo* mDemoSceneStateSkipDemo = nullptr;
    SceneAudioSystemPauseController* mSceneAudioSystemPauseController = nullptr;
    al::WipeHolder* mWipeHolder = nullptr;
};

static_assert(sizeof(WorldWarpHoleScene) == 0x1d0, "WorldWarpHoleScene size");
