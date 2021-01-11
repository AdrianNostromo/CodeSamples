#include <rpg3D/gw/entity/module/TeamModule.h>
#include "GPManAZMobs.h"
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplate_Battle_WESN_A_V1.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <rpg3D/gw/config/logicBlock/mob/LB_Mob_ImpactChaser_A_V1.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace battlezones_a_v1;

GPManAZMobs::GPManAZMobs(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGamePartMain::GWEvent_onEntityAdded->type, base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type,
		std::bind(&GPManAZMobs::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPManAZMobs::onActiveZoneChanged() {
	super::onActiveZoneChanged();

	mobsCount = 0;

	std::shared_ptr<base::IWrappedValue> val = base::IWrappedValue::new_bool(false);
	if (activeZoneFlags.test(ZoneTemplate_Battle_WESN_A_V1::FLAG_isZoneCleared, val, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/)) {
		spawnZoneUnits();
	}
}

void GPManAZMobs::spawnZoneUnits() {
	ArrayList<ZoneConfig::UnitSpawnConfig>* mobsSpawnsList = activeZoneConfig->getMobsSpawnsList();
	for(int i=0;i<mobsSpawnsList->count();i++) {
		ZoneConfig::UnitSpawnConfig* unitSpawnConfig = mobsSpawnsList->getPointer(i);

		EntityTemplate* entityTemplate = EntityTemplate::fromId(unitSpawnConfig->entityTemplateNSpace, unitSpawnConfig->entityTemplateId);
		if(entityTemplate == nullptr) {
			throw LogicException(LOC);
		}

		IWorldEntity*mobEntity = gw->getGPMain()->createEntity(
			entityTemplate,
			unitSpawnConfig->pos,
			&unitSpawnConfig->rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr,//instanceId
			0,//ownerPlayerId
			nullptr,//extraData
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[&unitSpawnConfig](IWorldEntity* entity) {
				rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				mob_team->setTeamIndex(rpg3D::TeamsIndexesList::MOB);

				entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayloadsList(
					&unitSpawnConfig->impactPayloadsList
				);

				rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

				rpg3D::LB_Mob_ImpactChaser_A_V1* lB_Mob_ImpactChaser_A_V1 = new rpg3D::LB_Mob_ImpactChaser_A_V1();
				entity_controller->registerLogicBlock(lB_Mob_ImpactChaser_A_V1, rpg3D::ILB_Mob_ImpactChaser_A_V1::ID, static_cast<rpg3D::ILB_Mob_ImpactChaser_A_V1*>(lB_Mob_ImpactChaser_A_V1));
				entity_controller->setSingleActiveGroupLogicBlock(rpg3D::LB_Mob_ImpactChaser_A_V1::GroupID, rpg3D::LB_Mob_ImpactChaser_A_V1::ID);
			}
		);
		(void)mobEntity;
	}
}

void GPManAZMobs::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == base::IGamePartMain::GWEvent_onEntityAdded->type) {
		rpg3D::ITeamModule* entity_team = event.getEntityData()->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		if(entity_team != nullptr && entity_team->getTeamIndex() == rpg3D::TeamsIndexesList::MOB) {
			mobsCount++;
		}
	}else if(event.type == base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
		rpg3D::ITeamModule* entity_team = event.getEntityData()->getComponentAs<rpg3D::ITeamModule*>(false/*mustExist*/);
		if(entity_team != nullptr && entity_team->getTeamIndex() == rpg3D::TeamsIndexesList::MOB) {
			mobsCount--;

			if(mobsCount < 0) {
				throw LogicException(LOC);
			}
			if(mobsCount == 0) {
				if(activeZoneConfig->zoneClearedEventsInstancePointers.count() > 0) {
					gw->dispatchEvents(&activeZoneConfig->zoneClearedEventsInstancePointers);
				}

				// Zone cleared.
				activeZoneFlags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isZoneCleared, true);
				onZoneFlagsChange();
			}
		}
	}else {
		throw LogicException(LOC);
	}
}

GPManAZMobs::~GPManAZMobs() {
	//void
}
