#pragma once

#include <base/gh.h>
#include "../generalActiveTool/ILBGeneralActiveTools.h"

namespace rpg3D {
namespace playerUnit {
class ILBGridTurnBasedActiveTools : virtual public ILBGeneralActiveTools {
	pub static int ID;
	pub int getId() final;

	//void

	pub ~ILBGridTurnBasedActiveTools() override = default;
};
};
};
