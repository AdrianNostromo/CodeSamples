#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>

namespace rpg3D {
class ILBActiveTools : virtual public ILogicBlock {
	pub static int GroupID;
	pub int getGroupID() final;

	//void

	pub ~ILBActiveTools() override = default;
};
};
