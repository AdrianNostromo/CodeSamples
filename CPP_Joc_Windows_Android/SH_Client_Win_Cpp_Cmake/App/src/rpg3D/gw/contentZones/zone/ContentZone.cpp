#include "ContentZone.h"
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/platform/IPlatformModule.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <worldGame3D/gw/content/IGPContent.h>

using namespace rpg3D;

ContentZone::ContentZone(
	std::string& id,
	bool useAutoPlayerPositioning, std::shared_ptr<std::string> defaultZoneEntrance)
	: super(id),
	useAutoPlayerPositioning(useAutoPlayerPositioning), defaultZoneEntrance(defaultZoneEntrance)
{
	//void
}

void ContentZone::onZoneActivatedPositionPlayer(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedPositionPlayer(zoneStartExtraData, gw);

	if (useAutoPlayerPositioning) {
		Vector3 playerSpawnPos{};
		IWorldEntity* spawnLocationEntity = nullptr;
		if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
			spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
			if (spawnLocationEntity == nullptr) {
				throw LogicException(LOC);
			}

			playerSpawnPos.set(spawnLocationEntity->getPos());
		} else {
			if (defaultZoneEntrance == nullptr) {
				// No default zone entrance target provided. This is required if zoneStartExtraData doesn't contain one.
				throw LogicException(LOC);
			}

			spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*defaultZoneEntrance);
			if (spawnLocationEntity == nullptr) {
				throw LogicException(LOC);
			}

			playerSpawnPos.set(spawnLocationEntity->getPos());
		}

		if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetOffset != nullptr) {
			playerSpawnPos.add(*zoneStartExtraData->teleportTargetOffset);
		}

		if (spawnLocationEntity != nullptr) {
			if (spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
				// Ignore the platform activation for a few frames if the player was spawned on it.
				gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(spawnLocationEntity->getInstanceId());
			}
		}

		gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);
		IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
		movingEntity->addAction_teleportToDestination(playerSpawnPos);
	}
}

ContentZone::~ContentZone() {
	//void
}
