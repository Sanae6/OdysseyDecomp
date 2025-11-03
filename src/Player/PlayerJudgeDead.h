#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class PlayerJudgeDead : public IJudge {
public:
    PlayerJudgeDead(const al::LiveActor* player);

    bool judge() const override;

    void reset() override {}

    void update() override {}

private:
    const al::LiveActor* mPlayer;
};
