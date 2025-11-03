#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "math/seadVectorFwd.h"

namespace al {
class AreaObj;
}

class IUsePlayerPuppet {
    virtual void start() = 0;
    virtual void end() = 0;
    virtual void cancel() = 0;
    virtual void setTrans(const sead::Vector3f& trans) = 0;
    virtual void setPose(const sead::Quatf& pose) = 0;
    virtual void setVelocity(const sead::Vector3f& velocity) = 0;
    virtual void resetPosition(const sead::Vector3f& position) = 0;
    virtual const sead::Vector3f& getTrans() const = 0;
    virtual const sead::Vector3f& getVelocity() const = 0;
    virtual const sead::Vector3f& getGravity() const = 0;
    virtual void calcFront(sead::Vector3f* front) const = 0;
    virtual void calcUp(sead::Vector3f* up) const = 0;
    virtual void startAction(const sead::SafeString& actionName) const = 0;
    virtual bool isActionEnd() const = 0;
    virtual bool isActionPlaying(const char* actionName) const = 0;
    virtual void setAnimRate(f32 rate) const = 0;
    virtual s32 getAnimFrameMax() const = 0;
    void startPlayerHitReaction(const char* hitReactionName);
    virtual void hide() = 0;
    virtual void show() = 0;
    virtual bool isHidden() const = 0;
    virtual void hideSilhouette() = 0;
    virtual void showSilhouette() = 0;
    virtual void hideShadow() = 0;
    virtual void showShadow() = 0;
    virtual void requestDamage() = 0;
    virtual void clearRequestDamage() = 0;
    virtual bool isRequestDamage() = 0;
    virtual void setBindEndOnGround() = 0;
    virtual bool isBindEndOnGround() = 0;
    virtual void setBindEndJump(const sead::Vector3f&, s32) = 0;
    virtual bool isBindEndJump() = 0;
    virtual void validateCollisionCheck() = 0;
    virtual void invalidateCollisionCheck() = 0;
    virtual bool isValidCollisionCheck() = 0;
    virtual bool isCollidedGround() = 0;
    virtual const sead::Vector3f& getCollidedGroundNormal() = 0;
};
