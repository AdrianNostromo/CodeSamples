#pragma once

#include <base/gh.h>
#include "../../groups/IToolMovement.h"

namespace rpg3D {
namespace playerUnit {
class IToolMDPadLocalFrontBack : virtual public rpg3D::IToolMovement {
	pub static int ID;
	pub int getId() final;

	//void

	pub ~IToolMDPadLocalFrontBack() override = default;
};
};
};
