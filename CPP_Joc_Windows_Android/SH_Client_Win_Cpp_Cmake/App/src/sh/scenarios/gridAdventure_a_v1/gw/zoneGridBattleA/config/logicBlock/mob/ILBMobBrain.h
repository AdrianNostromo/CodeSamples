#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class ILBMobBrain : virtual public rpg3D::ILogicBlock {
	pub static int GroupID;
	pub int getGroupID() final;

    pub ~ILBMobBrain() override = default;
};
};
};
