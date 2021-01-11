#include "AutoDestroyDurationModule.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include "../../template/autoDestroyDuration/AutoDestroyDurationTemplate.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/event/IPluggableEventsDispatcher.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>

using namespace rpg3D;

AutoDestroyDurationModule::AutoDestroyDurationModule(
	IWorldEntity* entity,
	AutoDestroyDurationTemplate* t)
	: super(entity, t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&AutoDestroyDurationModule::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void AutoDestroyDurationModule::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	if (entity->getIsMarkedForRemoval()) {
		return;
	}

	totalDurationS += event.deltaS;

	Vector3 posChangeDelta{};
	if (!isLastTickPosInit) {
		isLastTickPosInit = true;

		posChangeDelta.set(0.0f, 0.0f, 0.0f);
	} else {
		posChangeDelta.set(e->getPos());
		posChangeDelta.sub(lastTickPos);
	}
	lastTickPos.set(e->getPos());

	totalDistTravelled += posChangeDelta.len();

	// Check expirations.
	if ((t->lifeDistM >= 0.0f && totalDistTravelled >= t->lifeDistM) || (t->lifeDurationS >= 0.0f && totalDurationS >= t->lifeDurationS)) {
		entity->getHandler()->markEntityForRemoval(entity, ENT_REM_REASON_PURPOSE_FINISHED, false);
	}
}

AutoDestroyDurationModule::~AutoDestroyDurationModule() {
	//void
}
