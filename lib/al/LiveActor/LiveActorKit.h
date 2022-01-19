#pragma once

#include "al/hio/HioNode.h"

namespace al {
class GraphicsInitArg;

class LiveActorKit : public al::HioNode {
public:
    LiveActorKit(int unk1, int unk2);

    void init();
    void initGraphics(const al::GraphicsInitArg& initArg);
    void initHitSensorDirector();
    void initGravityHolder();
    void initShadowDirector();
    void initEffectSystem();
    void initSwitchAreaDirectory();
    void initNatureDirector();
    void endInit();
    void update();
    void clearGraphicsRequest();
    void updateGraphics();
    void preDrawGraphics();


};
}  // namespace al