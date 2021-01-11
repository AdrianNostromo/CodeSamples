#include "OGPMobsSpawnSystem.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/zoneFlags/IOGPZoneFlags.h>
#include <base/container/flags/ManagedFlags.h>
#include <base/container/flags/FlagEntry.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>
#include <worldGame3D/gw/entity/util/WorldEntitySpawnConfig.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>

using namespace rpg3D;

OGPMobsSpawnSystem::OGPMobsSpawnSystem(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void OGPMobsSpawnSystem::createBMain() {
	super::createBMain();

	gw->addEventListener(
		worldGame3D::IContentZone::GWEvent_onZoneActivated->type, base::IGamePartMain::GWEvent_onEntityAdded->type, base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type,
		std::bind(&OGPMobsSpawnSystem::onGWEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gpZoneFlags = gw->getComponentAs<IOGPZoneFlags*>(true/*mustExist*/);
}

void OGPMobsSpawnSystem::initMobsSpawnInformation(int mobTeam, std::shared_ptr<ArrayList<worldGame3D::WorldEntitySpawnConfig>> mobsSpawnList) {
	this->mobTeam = mobTeam;
	this->mobsSpawnList = mobsSpawnList;
}

void OGPMobsSpawnSystem::spawnZoneMobs() {
	if (mobsSpawnList == nullptr) {
		return;
	}

	for (int i = 0; i < mobsSpawnList->count(); i++) {
		worldGame3D::WorldEntitySpawnConfig* unitSpawnConfig = mobsSpawnList->getPointer(i);

		EntityTemplate* entityTemplate = EntityTemplate::fromId(unitSpawnConfig->entityTemplateNSpace, unitSpawnConfig->entityTemplateId);
		if (entityTemplate == nullptr) {
			throw LogicException(LOC);
		}


		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityTemplate,
			unitSpawnConfig->pos,
			&unitSpawnConfig->rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr/*instanceId*/, 0/*ownerPlayerId*/,
			nullptr/*extraData*/,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[unitSpawnConfig](IWorldEntity* entity) -> void {
				if (unitSpawnConfig->team != nullptr) {
					rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
					mob_team->setTeamIndex(*unitSpawnConfig->team);
				}
				 
				if (unitSpawnConfig->impactPayloadsList != nullptr) {
					entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayloadsList(
						unitSpawnConfig->impactPayloadsList.get()
					);
				}

				if (unitSpawnConfig->logicBlock != nullptr) {
					rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
					entity_controller->registerLogicBlock(unitSpawnConfig->logicBlock, unitSpawnConfig->logicBlock->getId(), unitSpawnConfig->logicBlockInterfaceRawPtr);
					entity_controller->setSingleActiveGroupLogicBlock(unitSpawnConfig->logicBlock->getGroupID(), unitSpawnConfig->logicBlock->getId());
				}
			}
		);
		(void)entity;
	}
}

void OGPMobsSpawnSystem::onGWEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == worldGame3D::IContentZone::GWEvent_onZoneActivated->type) {
		if (isMobsSpawnStarted) {
			throw LogicException(LOC);
		}
		isMobsSpawnStarted = true;

		isMobsSpawnFinished = false;
		activeMobsCount = 0;

		if (!isMobsSpawnFinished) {
			spawnZoneMobs();

			isMobsSpawnFinished = true;
		}
	}else if (event.type == base::IGamePartMain::GWEvent_onEntityAdded->type) {
		rpg3D::ITeamModule* entity_team = event.getEntityData()->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		if (entity_team != nullptr && entity_team->getTeamIndex() == mobTeam) {
			activeMobsCount++;
		}
	} else if (event.type == base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
		rpg3D::ITeamModule* entity_team = event.getEntityData()->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		if (entity_team != nullptr && entity_team->getTeamIndex() == mobTeam) {
			activeMobsCount--;

			if (activeMobsCount < 0) {
				throw LogicException(LOC);
			}
			if (activeMobsCount == 0 && isMobsSpawnFinished) {
				// Zone cleared.
				gpZoneFlags->getZoneFlags()->setFlag(FLAG_isZoneCleared, true);
			}
		}
	} else {
		throw LogicException(LOC);
	}
}

OGPMobsSpawnSystem::~OGPMobsSpawnSystem() {
	//void
}
