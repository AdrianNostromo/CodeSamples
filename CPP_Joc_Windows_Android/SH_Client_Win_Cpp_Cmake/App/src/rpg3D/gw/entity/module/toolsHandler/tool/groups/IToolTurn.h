#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/ITool.h>

namespace rpg3D {
class IToolTurn : virtual public ITool {
	pub static int GroupID;
	pub int getGroupID() final;

	//void

	pub ~IToolTurn() override = default;
};
};
