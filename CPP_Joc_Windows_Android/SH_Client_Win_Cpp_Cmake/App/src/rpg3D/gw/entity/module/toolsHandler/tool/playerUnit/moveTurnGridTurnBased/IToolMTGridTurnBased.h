#pragma once

#include <base/gh.h>
#include "../../groups/IToolMovement.h"

namespace base {
	class EntityEvent;
};

namespace rpg3D {
namespace playerUnit {
class IToolMTGridTurnBased : virtual public rpg3D::IToolMovement {
	pub static int ID;
	pub int getId() final;

	pub static base::EntityEvent* EVENT_gridMovement_skipped;
	pub static base::EntityEvent* EVENT_gridMovement_started;
	pub static base::EntityEvent* EVENT_gridMovement_finished;

	//void

	pub ~IToolMTGridTurnBased() override = default;
};
};
};
