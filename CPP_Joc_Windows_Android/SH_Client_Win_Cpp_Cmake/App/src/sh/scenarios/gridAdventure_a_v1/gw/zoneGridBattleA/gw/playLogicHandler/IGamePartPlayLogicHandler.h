#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class IGamePartPlayLogicHandler : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

    pub ~IGamePartPlayLogicHandler() override = default;
};
};
};
