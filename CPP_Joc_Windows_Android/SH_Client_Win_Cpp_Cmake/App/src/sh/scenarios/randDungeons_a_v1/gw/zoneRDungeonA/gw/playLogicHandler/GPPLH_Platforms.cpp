#include "GPPLH_Platforms.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <base/util/StringUtil.h>
#include "../../../zoneMain/config/ContentZoneMain.h"
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include "../../config/ContentZoneRDungeonA.h"
#include <rpg3D/gw/manager/IGPManager.h>
#include "../../config/util/ZoneStartExtraData.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

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
		if (event.getEntityData()->getInstanceId() == EntityIds::teleportDungeonEntryPoint || event.getEntityData()->getInstanceId() == EntityIds::teleporterExitDungeon) {
			// Exit the dungeon.
			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			
			std::shared_ptr<common::ZoneStartExtraData> zoneStartExtraData(new common::ZoneStartExtraData(
				getExitZonePlayerPosEntityId(), nullptr/*teleportTargetOffset*/,
				nullptr/*zeroHpZoneName*/, nullptr/*zeroHpZonePlayerPosEntityId*/
			));

			gpContentZones->setActiveZone(
				*getExitZoneName(), zoneStartExtraData, false/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		}else if (StringUtil::indexOf(*event.getEntityData()->getInstanceId(), EntityIds::teleportPrefix) == 0) {
			int teleportTargetZoneLevelIndex = event.getEntityData()->getExtraDataMustExistB("teleportTargetZoneLevelIndex")->getDirectAs_int();
			std::string teleportTargetEntityInstanceID = event.getEntityData()->getExtraDataMustExistB("teleportTargetEntityInstanceID")->getDirectAs_String();

			worldGame3D::IGamePartContentZones* gpContentZones = gw->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);

			if (teleportTargetZoneLevelIndex == -1) {
				throw LogicException(LOC);
			}
			
			// Go to a speciffic level zone.
			std::shared_ptr<DungeonContent> dungeonContent = getDungeonContent();
			std::shared_ptr<ZoneStartExtraData> zoneStartExtraData(new ZoneStartExtraData(
				!teleportTargetEntityInstanceID.empty() ? std::make_shared<std::string>(teleportTargetEntityInstanceID) : nullptr, nullptr/*teleportTargetOffset*/,
				dungeonContent, teleportTargetZoneLevelIndex/*levelZoneToActivate*/,
				getExitZoneName(), getExitZonePlayerPosEntityId(),
				getZeroHpZoneName(), getZeroHpZonePlayerPosEntityId()
			));
			gpContentZones->setActiveZone(
				ContentZoneRDungeonA::NAME, zoneStartExtraData, true/*restartZoneIfActive*/,
				true/*useDelayedOnLoopActivation*/
			);
		}
	} else {
		throw LogicException(LOC);
	}
}

GPPLH_Platforms::~GPPLH_Platforms() {
	//void
}
