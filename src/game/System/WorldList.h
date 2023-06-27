#pragma once

#include <container/seadPtrArray.h>
#include <container/seadStrTreeMap.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

struct StagePosInfo {

};

struct StageDBEntry {
    sead::FixedSafeString<0x80> stageName;
    sead::FixedSafeString<0x40> stageCategory;
    int useScenario;
};

struct WorldListEntry {
    const char* mainStageName;
    const char* worldDevelopName;
    int questInfoCount;
    int clearMainScenario;
    int endingScenario;
    int moonRockScenario;
    int* mainQuestIndexes;
    sead::PtrArray<StageDBEntry> stageNames;
};

class WorldList {
private:
    sead::PtrArray<WorldListEntry> mWorldList;
    sead::StrTreeMap<128, StagePosInfo*> mStagePosList;
    sead::PtrArrayImpl field_30;
public:
    WorldList(void);
    void getWorldNum(void);
    void getMainQuestMin(int, int);
    const char *getMainStageName(int) const;
    int tryFindWorldIndexByMainStageName(char const *stageName) const;
    int tryFindWorldIndexByStageName(char const *stageName) const;
    int tryFindWorldIndexByDevelopName(char const *stageName) const;
    void isEqualClearMainScenarioNo(int, int);
    void getAfterEndingScenarioNo(int);
    void isEqualAfterEndingScenarioNo(int, int);
    int getMoonRockScenarioNo(int worldId) const;
    bool isEqualMoonRockScenarioNo(int, int) const;
    const char *getWorldDevelopName(int) const;
    void getWorldScenarioNum(int);
    int findUseScenarioNo(const char *stageName) const;
    void checkNeedTreasureMessageStage(char const *);
    bool checkIsMainStage(char const *stageName) const;
    void tryFindTransOnMainStageByStageName(sead::Vector3f *, char const *, int);
    void tryFindHintTransByScenarioNo(sead::Vector3f *, int, int);
};

static_assert(sizeof(WorldList) == 0x40, "WorldList Size");