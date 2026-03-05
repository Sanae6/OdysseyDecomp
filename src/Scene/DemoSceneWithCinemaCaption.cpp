#include "Scene/DemoSceneWithCinemaCaption.h"
#include "Layout/CinemaCaption.h"
#include "Library/Nerve/NerveUtil.h"
#include "Util/DemoUtil.h"

DemoSceneWithCinemaCaption::DemoSceneWithCinemaCaption(al::WipeHolder* wipeHolder)
    : DemoScene(wipeHolder) {}

void DemoSceneWithCinemaCaption::init(const al::SceneInitInfo& initInfo) {
    DemoScene::init(initInfo);

    mName.format("DemoSceneWithCinemaCaption");
    mCaptionInfoHolder = rs::createCaptionInfoHolder(mStageName.cstr());
}

void DemoSceneWithCinemaCaption::initLayout(const al::LayoutInitInfo& initInfo) {
    DemoScene::initLayout(initInfo);

    mCinemaCaption = new CinemaCaption(initInfo);
}

void DemoSceneWithCinemaCaption::exePlay() {
    if (al::isFirstStep(this))
        mCinemaCaption->tryAppear(this->mCaptionInfoHolder);

    DemoScene::exePlay();
}
