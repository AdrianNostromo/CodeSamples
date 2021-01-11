#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>

namespace towerDefense_a_v1 {
namespace mobUnit {
class ILB_GridMob_A_V1 : virtual public rpg3D::ILogicBlock {
	pub static int ID;
	pub int getId() final;

	pub static int GroupID;
	pub int getGroupID() final;

	//void

	pub ~ILB_GridMob_A_V1() override = default;
};
};
};
