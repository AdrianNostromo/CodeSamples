#pragma once

#include <base/gh.h>
#include "../ILBGridTurnBasedMobBrain.h"

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class ILB_Mob_Chicken_TB_A_V1 : virtual public ILBGridTurnBasedMobBrain {
	pub static int ID;
	pub int getId() final;

	//void

	pub ~ILB_Mob_Chicken_TB_A_V1() override = default;
};
};
};
