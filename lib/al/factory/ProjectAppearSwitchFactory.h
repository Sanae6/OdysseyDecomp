#pragma once

#include "al/factory/Factory.h"

namespace al {

class ProjectAppearSwitchFactory : public al::ActorFactory {
public:
    ProjectAppearSwitchFactory();
    const char* convertName(const char*) const override;
};

}  // namespace al