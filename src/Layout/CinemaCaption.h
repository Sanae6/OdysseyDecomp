#pragma once

#include "Library/Layout/LayoutActor.h"
#include "container/seadPtrArray.h"

namespace al {
class LayoutInitInfo;
}
class CaptionInfoHolder;
class CaptionInfo;

class CinemaCaption : public al::LayoutActor {
public:
    CinemaCaption(const al::LayoutInitInfo& initInfo);

    void tryAppear(CaptionInfoHolder*);
    void setCaptionMsg(const char16* message);
    void calcShowFrame();
    void tryEnd();
    void updateMessageCore();
    void trySetNextCaptionMsg(CaptionInfo*);
    void tryHideCurrentCaptionMsg(CaptionInfo*);

    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    CaptionInfoHolder* mCaptionInfoHolder;
    CaptionInfo* mCurrentCaptionInfo;
    s32 mCaptionIndex;
    s32 mCaptionTime;
};

class CaptionInfoHolder {
public:
    CaptionInfoHolder(const char* captionList);

private:
    sead::PtrArray<CaptionInfo> captionInfos;
};

namespace CinemaCaptionFunction {
CaptionInfoHolder* createCaptionInfoHolder();
}
