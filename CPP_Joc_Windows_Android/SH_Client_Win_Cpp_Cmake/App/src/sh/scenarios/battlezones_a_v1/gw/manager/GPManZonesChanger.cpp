#include <sh/scenarios/battlezones_a_v1/gw/content/IGPContent.h>
#include "GPManZonesChanger.h"
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace battlezones_a_v1;

GPManZonesChanger::GPManZonesChanger(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&GPManZonesChanger::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPManZonesChanger::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		IWorldEntity* platformEntity = event.getEntityData();

		if (platformEntity->getInstanceId() == EntityIds::platform_Teleporter_Home) {
			gw->getHandler()->onGWEvent(GWE_RequestStartScenario_StartArea, nullptr);
		} else {
			bool isZonePrecleared;
			if (platformEntity->getInstanceId() == EntityIds::platform_zoneTeleporter_previous) {
				isZonePrecleared = true;
			}else {
				isZonePrecleared = false;
			}

			// Get the destination from the teleporters map.
			ZoneConfig* zoneConfig = getActiveZoneConfig();
			TeleporterTarget* teleporterTarget = zoneConfig->getTeleporterTarget(*platformEntity->getInstanceId());
			if (teleporterTarget == nullptr) {
				throw LogicException(LOC);
			}

			setActiveZone(
				teleporterTarget->zonesGroupId, teleporterTarget->zoneName,
				teleporterTarget->spawnLocationEntityId,
				isZonePrecleared,
				true
			);
		}
	}else {
		throw LogicException(LOC);
	}
}

GPManZonesChanger::~GPManZonesChanger() {
	//void
}
