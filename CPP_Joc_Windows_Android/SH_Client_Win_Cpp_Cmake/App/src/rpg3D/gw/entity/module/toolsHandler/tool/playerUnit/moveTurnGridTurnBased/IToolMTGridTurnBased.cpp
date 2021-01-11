#include "IToolMTGridTurnBased.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

int IToolMTGridTurnBased::ID = GetNew_ID();

int IToolMTGridTurnBased::getId() {
	return ID;
}

base::EntityEvent* IToolMTGridTurnBased::EVENT_gridMovement_skipped = new base::EntityEvent(false, "EVENT_gridMovement_skipped");
base::EntityEvent* IToolMTGridTurnBased::EVENT_gridMovement_started = new base::EntityEvent(false, "EVENT_gridMovement_started");
base::EntityEvent* IToolMTGridTurnBased::EVENT_gridMovement_finished = new base::EntityEvent(false, "EVENT_gridMovement_finished");
