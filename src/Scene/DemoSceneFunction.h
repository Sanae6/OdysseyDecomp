#include <math/seadVector.h>

namespace al {
class Scene;
class DemoActorHolder;
class WipeHolder;
}  // namespace al

class DemoSceneStateSkipDemo;

namespace DemoSceneFunction {
void updateKitWithDemoActorHolder(al::Scene* scene, al::DemoActorHolder* demoActorHolder,
                                  const char*, const char*, const char*, const sead::Vector3f*);
void updateKitWithDemoActorHolderSequence(al::Scene* scene, al::DemoActorHolder* demoActorHolder,
                                          const char*, const char*, const char*,
                                          const sead::Vector3f*);
void updateKitWithDemoActorHolderSequenceDemoChangeWorldScene(al::Scene* scene,
                                                              al::DemoActorHolder* demoActorHolder,
                                                              const char*, const char*, const char*,
                                                              const sead::Vector3f*);
void tryStartDemoEndFade(al::WipeHolder*, const char*, DemoSceneStateSkipDemo*,
                         const al::DemoActorHolder*);
}  // namespace DemoSceneFunction
