#pragma once

#include "Scene/DemoScene.h"

class CinemaCaption;
class CaptionInfoHolder;

class DemoSceneWithCinemaCaption : public DemoScene {
public:
    DemoSceneWithCinemaCaption(al::WipeHolder* wipeHolder);

    DemoSceneWithCinemaCaption() : DemoSceneWithCinemaCaption(nullptr) {}

    void init(const al::SceneInitInfo& initInfo) override;
    void initLayout(const al::LayoutInitInfo& initInfo) override;

    void exePlay() override;

public:
    CinemaCaption* mCinemaCaption = nullptr;
    CaptionInfoHolder* mCaptionInfoHolder = nullptr;
};
