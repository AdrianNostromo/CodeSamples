#pragma once

#include <base/gh.h>
#include "../../groups/IToolTurn.h"

namespace rpg3D {
namespace playerUnit {
class IToolTDPASSY_LMOSP : virtual public rpg3D::IToolTurn {
	pub static int ID;
	pub int getId() final;

	//void

	pub ~IToolTDPASSY_LMOSP() override = default;
};
};
};
