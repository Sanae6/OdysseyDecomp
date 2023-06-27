#pragma once

#include <al/Library/Resource/Resource.h>
#include <heap/seadFrameHeap.h>

#include "al/Library/Thread/AsyncFunctorThread.h"
#include "game/System/GameDataHolder.h"

class WorldResourceLoader {
private:

    al::AsyncFunctorThread* mWorldResourceLoader = nullptr; // WorldResourceLoader::loadResource
    sead::Heap* mWorldResourceHeap = nullptr;
    sead::FrameHeap* mCapWorldHeap = nullptr;
    sead::FrameHeap* mWaterfallWorldHeap = nullptr;
    s32 mLoadWorldId = -1;
    s32 mScenarioNo = -1;
    bool mIsScenarioRes = true;
    bool mIsCancelled = false;
    void* unkPtr4 = nullptr;
    void* unkPtr5 = nullptr;
    s32 unkInt3 = 0;
    bool mIsLoadResTable = true;
    s32 mCurLoadCount = 0;
    s32 mMaxLoadCount = 1;
    GameDataHolder* mDataHolder = nullptr;
    bool mIsLoadedPlayerModel = false;
    s32 unkInt6 = 0;

public:
    WorldResourceLoader(GameDataHolder *);
    virtual ~WorldResourceLoader();
    void loadResource(void);
    void cancelLoadWorldResource(void);
    void tryDestroyWorldResource(void);
    bool requestLoadWorldHomeStageResource(s32 worldIndex, s32 scenario);
    bool isEndLoadWorldResource(void) const;
    void requestLoadWorldResourceCommon(s32);
    bool requestLoadWorldResource(s32);
    void createResourcePlayer(void);
    void tryDestroyWorldResourceOnlyCap(void);
    float calcLoadPercent(void) const;
    s32 getLoadWorldId(void) const;
    al::Resource *tryLoadResource(char const*,char const*,char const*);
    void loadWorldResource(s32,s32,bool,char const*);
    float calcWorldResourceHeapSize(void) const;
};
