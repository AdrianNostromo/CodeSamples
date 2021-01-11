#include "GPPLH_Platforms.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/mob/Template_Mob_General_A_V1.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/TeamIndexesList.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <rpg3D/gw/manager/IGPManager.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::PathTD;

GPPLH_Platforms::GPPLH_Platforms(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type,
		std::bind(&GPPLH_Platforms::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPPLH_Platforms::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == rpg3D::IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		if (event.getEntityData()->getInstanceId() == EntityIds::mobsDestination) {
			// The despawn platforms.

			IWorldEntity* contactedEntity = event.getEntityData();

			if (contactedEntity->getInstanceId() == EntityIds::gridMob) {
				// Mob entity that reached the end and needs to be despawned.
				contactedEntity->markEntityForRemoval(ERR_DESTINATION_REACHED, false);
			}
		}
	} else {
		throw LogicException(LOC);
	}
}

GPPLH_Platforms::~GPPLH_Platforms() {
	//void
}
