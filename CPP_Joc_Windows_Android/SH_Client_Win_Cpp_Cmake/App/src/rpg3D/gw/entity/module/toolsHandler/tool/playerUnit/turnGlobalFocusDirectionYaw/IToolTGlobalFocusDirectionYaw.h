#pragma once

#include <base/gh.h>
#include "../../groups/IToolTurn.h"

namespace rpg3D {
namespace playerUnit {
class IToolTGlobalFocusDirectionYaw : virtual public rpg3D::IToolTurn {
	pub static int ID;
	pub int getId() final;

	//void

	pub ~IToolTGlobalFocusDirectionYaw() override = default;
};
};
};
