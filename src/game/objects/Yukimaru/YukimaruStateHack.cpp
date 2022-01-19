#include "game/objects/Yukimaru/YukimaruStateHack.h"

YukimaruStateHack::YukimaruStateHack(Yukimaru* host) : al::HostStateBase<Yukimaru>("憑依", host) {}



void YukimaruStateHack::exeWaitEnterMario() {}
void YukimaruStateHack::exeHackStart() {}
void YukimaruStateHack::exeMove() {}
void YukimaruStateHack::exeCancel() {}
void YukimaruStateHack::exeDamage() {}
void YukimaruStateHack::exeKill() {}
void YukimaruStateHack::exeTalkDemo() {}

namespace {
NERVE_IMPL(YukimaruStateHack, WaitEnterMario)
NERVE_IMPL(YukimaruStateHack, HackStart)
NERVE_IMPL(YukimaruStateHack, Move)
NERVE_IMPL(YukimaruStateHack, Cancel)
NERVE_IMPL(YukimaruStateHack, Damage)
NERVE_IMPL(YukimaruStateHack, Kill)
NERVE_IMPL(YukimaruStateHack, TalkDemo)
}
