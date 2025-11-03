#include <math/seadMatrix.h>

namespace al {
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al
class PlayerModelHolder;
class IPlayerModelChanger;

class PlayerEffect {
public:
    PlayerEffect(al::LiveActor*, const PlayerModelHolder*, const sead::Matrix34f*);
    void emitEffectWaterInOut(const sead::Matrix34f&, bool);
    void updateWaterSurfaceMtx(const al::WaterSurfaceFinder*);
    void tryEmitInvincibleEffect();
    void suspendInvincibleEffect();
    void tryDeleteInvincibleEffect();
    void restartInvincibleEffect();
    void updateInvincibleEffect(const IPlayerModelChanger*, bool);
    void clearRunEffect();
    bool isRunEffectDashFast() const;
    void tryStartRunEffectRunStart();
    void tryStartRunEffectRun();
    void tryStartRunEffectDash();
    void tryStartRunEffectDashFast();
    void tryStartRunEffectDashWaterSurface();
    void tryEmitRollingEffect();
    void tryDeleteRollingEffect();
    void clearStainEffect();
    void tryDeleteWetEffect();
    void tryDeleteStainPoisonEffect();
    void tryDeleteStainFireEffect();
    void tryDeleteStainIceEffect();
    void tryEmitWetEffect();
    void tryEmitStainPoisonEffect();
    void tryEmitStainFireEffect();
    void tryEmitStainIceEffect();
    void tryEmitSandSinkEffect(const sead::Vector3f&, const sead::Vector3f&, bool);
    void tryDeleteSandSinkEffect();
    void tryDeleteDamageFireRunEffect();

    void set_ba(bool value) { _ba = value; }

private:
    al::LiveActor* mPlayer;
    PlayerModelHolder* mModelHolder;
    sead::Matrix34f mWaterSurfaceEffectMtx;
    sead::Matrix34f mWaterSurfaceInOutEffectMtx;
    sead::Matrix34f mSandEffectMtx;
    al::LiveActor* mInvincibleModel2D;
    bool mIsInvincible;
    bool mIsInvincibleSuspended;
    bool _ba;
    s32 _bc;
};
