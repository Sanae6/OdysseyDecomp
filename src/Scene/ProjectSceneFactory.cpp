#include "Scene/ProjectSceneFactory.h"

#include "Library/Scene/CreateSceneFunc.h"

#include "Scene/DemoScene.h"
#include "Scene/DemoSceneWithCinemaCaption.h"
#include "Scene/FirstSequenceScene.h"
#include "Scene/StageScene.h"
#include "Scene/TitleMenuScene.h"

const al::NameToCreator<alSceneFunction::SceneCreatorFunction> sProjectSceneFactoryEntries[] = {
    {"DemoChangeWorldScene", nullptr},
    {"DemoScene", alSceneFunction::createSceneFunc<DemoScene>},
    {"DemoSceneWithCinemaCaption", alSceneFunction::createSceneFunc<DemoSceneWithCinemaCaption>},
    {"EndingScene", nullptr},
    {"FirstSequenceScene", alSceneFunction::createSceneFunc<FirstSequenceScene>},
    {"StageScene", alSceneFunction::createSceneFunc<StageScene>},
    {"StaffRollScene", nullptr},
    {"TitleMenuScene", alSceneFunction::createSceneFunc<TitleMenuScene>},
    {"WorldWarpHoleScene", nullptr}};

ProjectSceneFactory::ProjectSceneFactory() : SceneFactory("シーン生成") {
    initFactory(sProjectSceneFactoryEntries);
}
