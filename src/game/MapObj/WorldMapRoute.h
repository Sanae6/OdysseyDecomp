#pragma once

#include <game/MapObj/WorldMapParts.h>

class WorldMapRoute : WorldMapParts {
private:
    sead::Vector3f mJointEnd;
    sead::Vector3f mJointMiddle;
    sead::Vector3f field_158;
public:
    WorldMapRoute *create(char const*,char const*,al::ActorInitInfo const&,sead::Matrix34<float> const*,float);
    WorldMapRoute(char const*);

    void calcPose(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float,bool,bool,bool,bool);
    void appear(void);
    void kill(void);
    void movement(void);
    void calcAnim(void);
    void openRoute(int);
    void exeWait(void);
    void exeOpen(void);
};