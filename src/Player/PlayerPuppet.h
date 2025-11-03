#include <math/seadVector.h>

#include "Player/IUsePlayerPuppet.h"

namespace al {
class LiveActor;
class AreaObj;
}  // namespace al
class HackCap;
class PlayerAnimator;
class IUsePlayerCollision;
class ActorDimensionKeeper;
class IPlayerModelChanger;
class WorldEndBorderKeeper;
class PlayerCounterForceRun;
class PlayerDamageKeeper;
class PlayerEffect;
class PlayerInput;
class PlayerConst;

class PlayerPuppet : public IUsePlayerPuppet {
    PlayerPuppet(al::LiveActor*, HackCap*, PlayerAnimator*, IUsePlayerCollision*,
                 ActorDimensionKeeper*, IPlayerModelChanger*, WorldEndBorderKeeper*,
                 PlayerCounterForceRun*, PlayerDamageKeeper*, PlayerEffect*, const PlayerInput*,
                 const PlayerConst*) {}

    void start() override;
    void end() override;
    void cancel() override;
    void setTrans(const sead::Vector3f& trans) override;
    void setPose(const sead::Quatf& pose) override;
    void setVelocity(const sead::Vector3f& velocity) override;
    void resetPosition(const sead::Vector3f& position) override;
    const sead::Vector3f& getTrans() const override;
    const sead::Vector3f& getVelocity() const override;
    const sead::Vector3f& getGravity() const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcUp(sead::Vector3f* up) const override;
    void startAction(const sead::SafeString& actionName) const override;
    bool isActionEnd() const override;
    bool isActionPlaying(const char* actionName) const override;
    void setAnimRate(f32 rate) const override;
    s32 getAnimFrameMax() const override;
    void hide() override;
    void show() override;
    bool isHidden() const override;
    void hideSilhouette() override;
    void showSilhouette() override;
    void hideShadow() override;
    void showShadow() override;
    void requestDamage() override;
    void clearRequestDamage() override;
    bool isRequestDamage() override;
    void setBindEndOnGround() override;
    bool isBindEndOnGround() override;
    void setBindEndJump(const sead::Vector3f&, s32) override;
    bool isBindEndJump() override;
    void validateCollisionCheck() override;
    void invalidateCollisionCheck() override;
    bool isValidCollisionCheck() override;
    bool isCollidedGround() override;
    const sead::Vector3f& getCollidedGroundNormal() override;

    void startPlayerHitReaction(const char* hitReactionName);
    void setBindEndWallJump(const sead::Vector3f&, s32);
    void validate2D();
    void keepOn2D();
    void endKeepOn2D();
    void requestUpdateRecoveryInfo(bool, bool, const sead::Vector3f&, sead::Vector3f&,
                                   const al::AreaObj*);
    bool tryUpdateRecoveryInfo(bool*, bool*, const sead::Vector3f*, sead::Vector3f*,
                               const al::AreaObj**);
    bool isBinding() const;
    bool isNoCollide() const;

    private:
    
};
