#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>
#include <prim/seadBitFlag.h>

class GameDataFile {
public:
    struct HintInfo {

        enum class HintStatus {
            NONE,
            NPC,
            AMIIBO
        };

    private:
        sead::FixedSafeString<0x80> mStageName;     // 0x0
        sead::FixedSafeString<0x80> mObjId;         // 0x98
        sead::FixedSafeString<0x40> mScenarioName;  // 0x130
        const char *mObjectName;                    // 0x188
        sead::Vector3f mTrans;                      // 0x190
        sead::Vector3f mTransAgain;                 // 0x19C
        void *unkPtr1;                              // 0x1A8
        void *unkPtr2;                              // 0x1B0
        void *unkPtr3;                              // 0x1B8
        void *unkPtr4;                              // 0x1C0
        s32 mMainScenarioNo;                        // 0x1C8
        s32 mWorldIndex;                            // 0x1CC
        bool mIsMoonRock;                           // 0x1D0
        bool unkBool1;                              // 0x1D1
        bool mIsAchievement;                        // 0x1D2
        bool mIsGrand;                              // 0x1D3
        bool mIsShopMoon;                           // 0x1D4
        s32 unkInt;                                 // 0x1D8
        HintStatus mHintStatus;                     // 0x1DC
        s32 unkInt3;                                // 0x1E0
        s32 unkInt4;                                // 0x1E4
        void *unkPtr6;                              // 0x1E8
        s32 mUniqueID;                              // 0x1F0
        s32 mHintIdx;                               // 0x1F4
        sead::FixedSafeString<0x20> mOptionalID;    // 0x1F8
        sead::BitFlag32 mProgressBitflag;           // 0x230
        bool unkBool2;                              // 0x234
        bool unkBool3;                              // 0x235
    public:
        void clear(void);

        bool isDisableByWorldWarpHole(bool) const;
        bool isEnableUnlock(int, bool, int, bool) const;
        bool isHintStatusUnlock(int, int, bool) const;
        bool isHintStatusUnlockByNpc(void) const;
        bool isHintStatusUnlockByAmiibo(void) const;
        bool isEnableNameUnlockByScenario(int, int, bool) const;

        bool testFunc(int, bool, int, bool) const;

    };
private:

};
