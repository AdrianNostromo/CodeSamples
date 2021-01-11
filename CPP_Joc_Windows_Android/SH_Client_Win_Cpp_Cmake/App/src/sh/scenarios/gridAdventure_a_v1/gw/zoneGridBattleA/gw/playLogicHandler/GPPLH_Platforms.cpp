#include "GPPLH_Platforms.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include "../../../zoneGridBattleA/config/ContentZoneGridBattleA.h"
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <memory>
#include <rpg3D/gw/manager/IGPManager.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneGridBattleA/config/util/ZoneStartExtraData.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <worldGame3D/gw/contentZones/zone/ContentZone.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

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
		if (event.getEntityData()->getInstanceId() == EntityIds::enterNextGridBattleLevel) {
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);

			std::shared_ptr<worldGame3D::ContentZone> activeContentZone = gpContentZones->getActiveContentZone();

			GridBattleA::ZoneStartExtraData* currentZoneStartExtraData = nullptr;
			if (activeContentZone->getZoneStartExtraData() != nullptr) {
				currentZoneStartExtraData = activeContentZone->getZoneStartExtraData()->getAs<GridBattleA::ZoneStartExtraData*>();
			}

			std::shared_ptr<GridBattleA::ZoneStartExtraData> newZoneStartExtraData = nullptr;
			if (currentZoneStartExtraData == nullptr) {
				// This was zoneLevel 0, next is 1;
				newZoneStartExtraData = std::make_shared<GridBattleA::ZoneStartExtraData>(
					nullptr/*teleportTargetEntityInstanceID*/, nullptr/*teleportTargetOffset*/,
					nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/,
					0 + 1/*zoneLevelIndex*/
				);
			} else {
				// Continue to next level.
				newZoneStartExtraData = std::make_shared<GridBattleA::ZoneStartExtraData>(
					nullptr/*teleportTargetEntityInstanceID*/, nullptr/*teleportTargetOffset*/,
					nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/,
					currentZoneStartExtraData->zoneLevelIndex + 1/*zoneLevelIndex*/
				);
			}

			gpContentZones->setActiveZone(
				GridBattleA::ContentZoneGridBattleA::NAME, newZoneStartExtraData/*zoneStartExtraData*/, true/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		} else {
			// Unhandled platform. Script platforms are already processed by the script logic and never get here.
			throw LogicException(LOC);
		}
	} else {
		throw LogicException(LOC);
	}
}

GPPLH_Platforms::~GPPLH_Platforms() {
	//void
}
