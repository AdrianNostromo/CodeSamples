#include "GPPLH_Platforms.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/roomgeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/roomgeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/roomgeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <memory>
#include <rpg3D/gw/manager/IGPManager.h>
#include "../../config/ContentZoneArenaRoom.h"
#include <sh/scenarios/roomgeons_a_v1/gw/zoneArenaRoom/config/util/ZoneStartExtraData.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

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
		if (event.getEntityData()->getInstanceId() == EntityIds::nextZoneInSequenceTeleporter) {
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			std::shared_ptr<worldGame3D::ContentZone> activeContentZone = gpContentZones->getActiveContentZone();
			ContentZoneArenaRoom* cActiveContentZone = dynamic_cast<ContentZoneArenaRoom*>(activeContentZone.get());

			int zoneInSequenceIndex = cActiveContentZone->getZoneInSequenceIndex();

			int nextZoneInSequenceIndex = zoneInSequenceIndex + 1;
			
			std::shared_ptr<ArenaRoom::ZoneStartExtraData> zoneStartExtraData(new ArenaRoom::ZoneStartExtraData(
				zoneInSequenceIndex/*zoneInSequenceIndex*/, nullptr/*teleportTargetOffset*/,
				nullptr/*teleportTargetEntityInstanceID*/,
				nullptr/*exitZoneName*/, nullptr/*exitZonePlayerPosEntityId*/
			));

			gpContentZones->setActiveZone(
				ArenaRoom::ContentZoneArenaRoom::NAME, zoneStartExtraData/*zoneStartExtraData*/, true/*restartZoneIfActive*/,
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
