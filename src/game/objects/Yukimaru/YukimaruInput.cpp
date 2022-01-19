#include "game/objects/Yukimaru/YukimaruInput.h"

bool YukimaruInput::isTriggerJump() const {
    return false;
}
bool YukimaruInput::isHoldJump() const {
    return false;
}
bool YukimaruInput::isSwingDirLeft() const {
    return false;
}
bool YukimaruInput::isSwingDirRight() const {
    return false;
}
void YukimaruInput::calcInputVec(sead::Vector3f* input) const {
    input->z = sead::Vector3f::zero.z;
    // TODO: fix this mess
    *(long*)&input->x = *(long*)&sead::Vector3f::zero.x;
}