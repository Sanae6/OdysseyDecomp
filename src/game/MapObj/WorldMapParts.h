#pragma once

#include <al/Library/LiveActor/LiveActor.h>
#include <math/seadMatrix.h>

class WorldMapParts : public al::LiveActor {
protected:
    const sead::Matrix34f* mWorldMtx = nullptr;
    sead::Matrix34f mLocalMtx = sead::Matrix34f::ident;
public:
    WorldMapParts(char const*);

    void setWorldMtx(sead::Matrix34f const&);
    void updatePose(void);
    void control(void) override;

    virtual void setLocalMtx(sead::Matrix34f const&);

    static void initParts(WorldMapParts*,char const*,al::ActorInitInfo const&,sead::Matrix34f const*,sead::Matrix34f const&,char const*);
    static WorldMapParts* create(char const*,char const*,al::ActorInitInfo const&,sead::Matrix34f const*,sead::Matrix34f const&,char const*);
};
