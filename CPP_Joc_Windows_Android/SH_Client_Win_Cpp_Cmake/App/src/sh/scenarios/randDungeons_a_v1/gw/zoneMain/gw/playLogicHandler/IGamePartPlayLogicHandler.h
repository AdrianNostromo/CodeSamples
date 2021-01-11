#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace randDungeons_a_v1 {
namespace Main {
class IGamePartPlayLogicHandler : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

    pub ~IGamePartPlayLogicHandler() override = default;
};
};
};
