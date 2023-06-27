#include <game/Scene/StageSceneStateWorldMap.h>



StageSceneStateWorldMap* StageSceneStateWorldMap::tryCreate(al::Scene*, const al::SceneInitInfo&, const al::ActorInitInfo&, const al::LayoutInitInfo&,
                                                            StageSceneStateCollectionList*) {
    return nullptr;
}

StageSceneStateWorldMap::StageSceneStateWorldMap(char const* name, al::Scene* host, ShineTowerRocket *shineTowerRocket, al::SceneInitInfo const& sceneInitInfo,
                                                 al::ActorInitInfo const& actorInitInfo, al::LayoutInitInfo const& lytInitInfo, StageSceneStateCollectionList* stateCollectList)
    : al::HostStateBase<al::Scene>(name, host) {

}

StageSceneStateWorldMap::~StageSceneStateWorldMap() {}

void StageSceneStateWorldMap::control(void) {}
void StageSceneStateWorldMap::appear(void) {}
void StageSceneStateWorldMap::kill(void) {}

void StageSceneStateWorldMap::exeInit(void) {}
void StageSceneStateWorldMap::exeDemoPrep(void) {}
void StageSceneStateWorldMap::exeDemoStart(void) {}
void StageSceneStateWorldMap::appearWorldSelectLayout(void) {}
void StageSceneStateWorldMap::exeDemoWorldOpen(void) {}
void StageSceneStateWorldMap::exeDemoWorldUnlock(void) {}
void StageSceneStateWorldMap::exeDemoWorldSelect(void) {}
void StageSceneStateWorldMap::exeDemoWorldComment(void) {}
void StageSceneStateWorldMap::exeWait(void) {}
void StageSceneStateWorldMap::exeIgnoreMessage(void) {}
void StageSceneStateWorldMap::exeCollectionListMain(void) {}
void StageSceneStateWorldMap::exeCollectionListExit(void) {}
void StageSceneStateWorldMap::exeExit(void) {}
void StageSceneStateWorldMap::exeEnd(void) {}

void StageSceneStateWorldMap::tryOpenWorldMap(void) {}
void StageSceneStateWorldMap::isCloseEndWipe(void) {}
void StageSceneStateWorldMap::isOpenEndWipe(void) {}
void StageSceneStateWorldMap::isOpenCollectionList(void) {}
