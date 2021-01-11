#include <sh/scenarios/battlezones_a_v1/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include "GPMan_PlayerUnitDeathHandling.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace battlezones_a_v1;

GPMan_PlayerUnitDeathHandling::GPMan_PlayerUnitDeathHandling(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		IGPContent::GWEvent_onSelectedPlayerEntityChanged->type,
		std::bind(&GPMan_PlayerUnitDeathHandling::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPMan_PlayerUnitDeathHandling::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == IGPContent::GWEvent_onSelectedPlayerEntityChanged->type) {
		playerDeathEventsListenersList.clear();

		IWorldEntity* playerEntity = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/);
		if(playerEntity != nullptr) {
			playerEntity->addEventListener(
				rpg3D::LivingEntityModule::EVENT_livingEntity_ZeroHp->type,
				std::bind(&GPMan_PlayerUnitDeathHandling::onPlayerEntityDeathEvent, this, std::placeholders::_1, std::placeholders::_2),
				autoListenersList
			);
		}
	}else {
		throw LogicException(LOC);
	}
}

void GPMan_PlayerUnitDeathHandling::onPlayerEntityDeathEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::LivingEntityModule::EVENT_livingEntity_ZeroHp->type) {
		// Set a flag to recreate the zones group.
		setRegenerateZonesGroupFlag();

		// Activate the starter area zone.
		setActiveZone(
			GLOBAL_ZONES_GROUP_ID, ZONE_ID_NEXUS,
			EntityIds::defaultSpawnLocation,
			false/*isZonePrecleared*/,
			true/*activateOnLoopTick*/
		);
	}else {
		throw LogicException(LOC);
	}
}

GPMan_PlayerUnitDeathHandling::~GPMan_PlayerUnitDeathHandling() {
	//void
}
