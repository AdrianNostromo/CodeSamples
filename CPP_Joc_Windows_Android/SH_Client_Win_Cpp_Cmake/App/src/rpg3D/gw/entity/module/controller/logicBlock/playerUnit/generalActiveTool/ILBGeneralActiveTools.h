#pragma once

#include <base/gh.h>
#include "../../activeTool/ILBActiveTools.h"

namespace rpg3D {
namespace playerUnit {
class ILBGeneralActiveTools : virtual public ILBActiveTools {
	pub static int ID;
	pub int getId() override;// This can still be extended further.

	//void

	pub ~ILBGeneralActiveTools() override = default;
};
};
};
