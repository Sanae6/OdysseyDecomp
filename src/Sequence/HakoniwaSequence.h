#pragma once

#include <basis/seadTypes.h>

#include "Library/Sequence/Sequence.h"

#include "System/GameDataHolderAccessor.h"

class HakoniwaStateDemoOpening;
class HakoniwaStateDemoEnding;
class HakoniwaStateDemoWorldWarp;
class HakoniwaStateSimpleDemo;
class HakoniwaStateBootLoadData;
class HakoniwaStateDeleteScene;
class WorldResourceLoader;
class StageScene;
class GameDataHolderAccessor;
class BootLayout;
class TimeBalloonSequenceInfo;
class CollectBgmPlayer;
class LoadLayoutCtrl;

namespace al {
class WipeHolder;
class Scene;
class AudioDirector;
class LayoutKit;
class SimpleLayoutAppearWaitEnd;
class AsyncFunctorThread;
class SeadAudioPlayer;
class AudioBusSendFader;
class SimpleAudioUser;
class ScreenCaptureExecutor;
class GamePadSystem;
class EffectSystem;

}  // namespace al

class HakoniwaSequence : public al::Sequence {
public:
    HakoniwaSequence(const char* name);
    ~HakoniwaSequence() override = default;

    void init(const al::SequenceInitInfo& initInfo) override;
    void update() override;
    void drawMain() const override;

    void updatePadSystem();
    void destroySceneHeap(bool destroyResource);
    void initSystem();
    bool isEnableSave() const;

    void exeBootLoadData();
    void exeDemoOpening();
    void exeLoadWorldResource();
    void exeLoadWorldResourceWithBoot();
    void exeLoadStage();
    void exePlayStage();
    void exeDemoWorldWarp();
    void exeDemoEnding();
    void exeDestroy();
    void exeMiss();
    void exeMissCoinSub();
    void exeMissEnd();
    void exeDemoLava();
    void exeFadeToNewGame();
    void exeChangeLanguage();
    void exeWaitWriteData();
    void exeWaitLoadData();
    void exeWaitWriteDataModeChange();
    void exeWaitLoadDataModeChange();

    bool isDisposable() const override;
    al::Scene* getCurrentScene() const override;

private:
    al::Scene* mCurrentScene = nullptr;
    GameDataHolderAccessor mGameDataHolder;
    al::GamePadSystem* mGamePadSystem = nullptr;
    HakoniwaStateDemoOpening* mStateDemoOpening = nullptr;
    HakoniwaStateDemoEnding* mStateDemoEnding = nullptr;
    HakoniwaStateDemoWorldWarp* mStateDemoWorldWarp = nullptr;
    HakoniwaStateSimpleDemo* mStateSimpleDemo = nullptr;
    HakoniwaStateBootLoadData* mStateBootLoadData = nullptr;
    HakoniwaStateDeleteScene* mStateDeleteScene = nullptr;
    al::LayoutKit* mLayoutKit = nullptr;
    bool mIsDestroyScene = true;
    sead::FixedSafeString<128> mStageName;
    s32 mNextScenarioNum = 0;
    al::ScreenCaptureExecutor* mScreenCaptureExecutor = nullptr;
    al::WipeHolder* mWipeHolder = nullptr;
    bool mIsMissEnd = false; 
    al::SimpleLayoutAppearWaitEnd* mCounterMiss = nullptr;
    s32 mCurrentCoins = 0;
    s32 mFinalCoins = 0;
    BootLayout* mBootLayout = nullptr;
    al::EffectSystem* mEffectSystem = nullptr;
    al::AsyncFunctorThread* mInitThread = nullptr;
    bool mIsInitialized = false;
    al::SeadAudioPlayer* mSeAudioPlayer = nullptr;
    al::SeadAudioPlayer* mBgmAudioPlayer = nullptr;
    al::AudioBusSendFader* mAudioBusSendFader = nullptr;
    WorldResourceLoader* mResourceLoader = nullptr;
    sead::Heap* mPlayerResourceHeap = nullptr;
    sead::FixedSafeString<128> mCapName;
    sead::FixedSafeString<128> mCostumeName;
    al::SimpleAudioUser* mPlayerAudioUser = nullptr;
    bool mIsHackEnd = false;
    bool mIsWarpCheckpoint = false;
    TimeBalloonSequenceInfo* mBalloonSeqInfo = nullptr;
    CollectBgmPlayer* mCollectBgmPlayer = nullptr;
    sead::FixedSafeString<128> mLanguage;
    s32 mFileId = -1;
    LoadLayoutCtrl* mLoadLayoutCtrl = nullptr;
    bool mIsKidsMode = false;
};

static_assert(sizeof(HakoniwaSequence) == 0x418, "HakoniwaSequnce size");
