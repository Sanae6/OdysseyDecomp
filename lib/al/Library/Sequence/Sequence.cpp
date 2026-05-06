#include "Library/Sequence/Sequence.h"
#include "Library/Audio/AudioDirector.h"
#include "Library/Audio/AudioDirectorInitInfo.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Scene/Scene.h"
#include "Library/System/GameSystemInfo.h"

namespace al {

Sequence::Sequence(const char* name) : NerveExecutor(name), mName(name) {}

void Sequence::init(const SequenceInitInfo& initInfo) {}

void Sequence::update() {
    if (mNextScene) {
        mCurrentScene = mNextScene;
        mNextScene = nullptr;
    }

    if (mCurrentScene && mCurrentScene->isAlive())
        mCurrentScene->movement();

    if (mAudioDirector)
        mAudioDirector->updatePre();
    updateNerve();
    if (mAudioDirector)
        mAudioDirector->updatePost();
}

void Sequence::kill() {
    mIsAlive = false;
}

void Sequence::drawMain() const {
    if (mCurrentScene && mCurrentScene->isAlive())
        mCurrentScene->drawMain();
}

void Sequence::drawSub() const {
    if (mCurrentScene && mCurrentScene->isAlive())
        mCurrentScene->drawSub();
}

bool Sequence::isDisposable() const {
    return true;
}

void Sequence::initAudio(const GameSystemInfo& gameSystemInfo, const char* stageName,
                         s32 maxRequests, s32 playerCount, s32 listenerCount, const char*) {
    mAudioDirector = new AudioDirector();
    AudioDirectorInitInfo initInfo{.audioSystemInfo =
                                       gameSystemInfo.audioSystem->getAudioSystemInfo(),
                                   .curStage = stageName,
                                   .seDirectorInitInfo = {
                                       .maxRequests = maxRequests,
                                       .playerCount = playerCount,
                                       .listenerCount = listenerCount,
                                   }};
    mAudioDirector->init(initInfo);
}

}  // namespace al
