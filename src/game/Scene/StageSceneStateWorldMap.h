#pragma once

#include <al/Library/Layout/LayoutActor.h>
#include <al/Library/LiveActor/ActorInitInfo.h>
#include <al/Library/Nerve/NerveStateBase.h>
#include <al/Library/Scene/Scene.h>

#include <container/seadPtrArray.h>
#include <game/MapObj/WorldMapParts.h>
#include <game/MapObj/WorldMapRoute.h>

class ShineTowerRocket;
class StageSceneStateCollectionList;
class TalkMessage;

namespace al {
class SceneInitInfo;
class SimpleLayoutAppearWaitEnd;
class WipeSimple;
class KeyRepeatCtrl;
}

class WorldMapPlayerIcon;
class WorldMapCamera;

class StageSceneStateWorldMap : public al::HostStateBase<al::Scene> {
private:

    al::SimpleLayoutAppearWaitEnd *mWorldSelectLyt;
    al::SimpleLayoutAppearWaitEnd *mWorldSelectMovieLyt;
    al::LayoutActor *mWorldSelectFooter;
    void* field_38;
    al::LayoutActor **mParWorldList;
    al::LayoutActor **mParIconList;
    void* field_50;
    al::WipeSimple *mWorldWipeSimple;
    al::WipeSimple *mWorldWipeCapture;
    TalkMessage *mWorldMsg;
    al::KeyRepeatCtrl *mKeyRepeatCtrl;
    al::Scene *mWorldMapStage;
    al::LiveActor *mWorldMapEarth;
    al::LiveActorGroup *mWorldMapGroup;
    WorldMapParts **mWorldMapPartsList;
    WorldMapRoute **mWorldMapRouteList;
    WorldMapRoute **mMapRouteGroup;
    WorldMapPlayerIcon *mPlayerIcon;
    al::LiveActor *mWorldMapKoopaShip;
    ShineTowerRocket *mShineTowerRocket;
    void* field_C0;
    char field_C8;
    char field_C9[47];
    void* field_F8;
    s32 mWorldIndexBoss;
    void* field_100;
    s32 mWorldNum;
    int field_108;
    s32* field_110;
    void* field_118;
    void* field_11C;
    void* field_120;
    void* field_128;
    void* field_130;
    WorldMapCamera *mWorldMapCamera;
    void* field_140;
    StageSceneStateCollectionList *mStateCollectionList;
    bool field_150;
    bool mIsGotAllShine;
    sead::PtrArrayImpl *mPtrArray;

public:

    StageSceneStateWorldMap(char const*, al::Scene *, ShineTowerRocket *, al::SceneInitInfo const&, al::ActorInitInfo const&,
                            al::LayoutInitInfo const&, StageSceneStateCollectionList *);
    ~StageSceneStateWorldMap() override;

    void control(void) override;
    void appear(void) override;
    void kill(void) override;

    void exeInit(void);
    void exeDemoPrep(void);
    void exeDemoStart(void);
    void appearWorldSelectLayout(void);
    void exeDemoWorldOpen(void);
    void exeDemoWorldUnlock(void);
    void exeDemoWorldSelect(void);
    void exeDemoWorldComment(void);
    void exeWait(void);
    void exeIgnoreMessage(void);
    void exeCollectionListMain(void);
    void exeCollectionListExit(void);
    void exeExit(void);
    void exeEnd(void);

    void tryOpenWorldMap(void);
    void isCloseEndWipe(void);
    void isOpenEndWipe(void);
    void isOpenCollectionList(void);

    static StageSceneStateWorldMap *tryCreate(al::Scene *, al::SceneInitInfo const&, al::ActorInitInfo const&, al::LayoutInitInfo const&, StageSceneStateCollectionList *);

};