#pragma once

#include <math/seadVector.h>
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
struct SceneInitInfo;
class DemoSyncedEventKeeper;
class WipeHolder;
class DemoActorHolder;
class WindowConfirm;
class KeyRepeatCtrl;
}  // namespace al

class SceneAudioSystemPauseController;
class PlayGuideSkip;

class DemoSceneStateSkipDemo : public al::HostStateBase<al::Scene> {
public:
    DemoSceneStateSkipDemo(al::Scene*, const al::SceneInitInfo&, bool skipConfirmation,
                           SceneAudioSystemPauseController*, al::DemoSyncedEventKeeper*,
                           al::WipeHolder*, bool);
    void appear();
    void kill();
    void appearPlayGuideSkip();
    void appearPlayGuideSkipForLoadingScene();
    void hidePlayGuideSkipIfShow();
    void tryChangeToSkipConfirm(const al::Nerve*);
    bool tryChangeToSkipConfirmForLoadingScene(const al::Nerve*, bool);
    void notifyStartDemoEndFade();
    bool isNerveFirst(const al::Nerve*) const;
    void tryOpenWipe();
    void tryRequestGraphicsForDemoChangeWorldScene();

    void exeWait();
    void exeSkipConfirm();
    void exeSkip();

    void setDemoActorHolder(al::DemoActorHolder* holder) { mDemoActorHolder = holder; }

    bool skipped() const { return mSkipped; }

    const al::Nerve* firstNerve() const { return mFirstNerve; }

private:
    al::WindowConfirm* mWindowConfirm = nullptr;
    PlayGuideSkip* mPlayGuideSkip = nullptr;
    al::KeyRepeatCtrl* mKeyRepeatCtrl = nullptr;
    bool mSkipConfirmation = false;
    const al::Nerve* mFirstNerve = nullptr;
    const al::Nerve* _48 = nullptr;
    bool mSkipped = false;
    bool mSkipForLoadingScene = false;
    SceneAudioSystemPauseController* mSceneAudioSystemPauseController;
    al::DemoSyncedEventKeeper* mDemoSyncedEventKeeper;
    al::WipeHolder* mWipeHolder;
    bool mUpdateDemoActors;
    al::DemoActorHolder* mDemoActorHolder;
    bool _60;
    char* _68;
    char* _70;
    char* _78;
    sead::Vector3f _80;
    bool mDemoEndFadeActive;
};
