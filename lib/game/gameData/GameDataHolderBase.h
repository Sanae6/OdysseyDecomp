#pragma once

namespace al {
class ISceneObj {};
};  // namespace al

#include "al/hio/HioNode.h"
#include "al/message/IUseMessageSystem.h"

class GameDataHolderBase : public al::ISceneObj, public al::IUseMessageSystem, public al::HioNode {
public:
};