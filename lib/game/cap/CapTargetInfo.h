#pragma once

#include "sead/include/math/seadVector.h"
#include "sead/include/math/seadMatrix.h"

namespace al {
class LiveActor;
}

class CapTargetInfo {
public:
    CapTargetInfo();
    void init(const al::LiveActor* actor, const char*) const;
    void setFollowLockOnMtx(char const*, sead::Vector3f const&, sead::Vector3f const&) const;
    void setLockOnStartAnimName(const char* name) const;
    void setLockOnAnimName(const char* name) const;
    void setHackName(const char* name) const;
    void makeLockOnMtx(sead::Matrix34f* mtx) const;
    void calcLockOnFollowTargetScale(sead::Vector3f*) const;
};
