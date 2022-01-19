#pragma once

#include "sead/include/math/seadVector.h"

class YukimaruInput {
public:
    virtual bool isTriggerJump() const;
    virtual bool isHoldJump() const;
    virtual bool isSwingDirLeft() const;
    virtual bool isSwingDirRight() const;
    virtual void calcInputVec(sead::Vector3f* input) const;
};
