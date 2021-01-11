#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace towerDefense_a_v1 {
namespace PathTD {
class IGamePartPlayLogicHandler : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual int getCurrentLives() = 0;
	pub virtual void setCurrentLives(int currentLives) = 0;
	
	pub virtual int getCurrentGold() = 0;
	pub virtual void setCurrentGold(int currentGold) = 0;

    pub ~IGamePartPlayLogicHandler() override = default;
};
};
};
