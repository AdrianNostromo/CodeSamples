#include "GPPLH_PlayLoop.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/mob/Template_Mob_General_A_V1.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/TeamIndexesList.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/structure/Template_BlockerPillar_A_V1.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/turret/Template_Turret_CubesCannon_A_V1.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/module/controller/logicBlock/turret_cubesCannon_A/LB_Turret_CubesCannon_A.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/module/customCubesCannonTurret/ICustomCubesCannonTurretModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/TeamIndexesList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/projectile/Template_Projectile_CannonCube_A_V1.h>
#include <sh/scenarios/towerDefense_a_v1/zones/pathTD/gw/views/mainui/IMainInGameUIView.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::PathTD;

const int GPPLH_PlayLoop::Status::ZERO = 0;
const int GPPLH_PlayLoop::Status::preStartDelay = 1;
const int GPPLH_PlayLoop::Status::waitingForNextWaveStartRequest = 2;
const int GPPLH_PlayLoop::Status::spawningWaveEnemies = 3;
const int GPPLH_PlayLoop::Status::waitingForWaveEnd = 4;
const int GPPLH_PlayLoop::Status::postWaveDelay = 5;

GPPLH_PlayLoop::GPPLH_PlayLoop(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_contentCreation->type,
		std::bind(&GPPLH_PlayLoop::onGWEvent_gameLoop_contentCreation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPPLH_PlayLoop::createBMain() {
	super::createBMain();

	// Set the uiView cb for the btn start next wave.
	mainUIVIew->setCb_onStartNextWaveRequest(std::bind(&GPPLH_PlayLoop::uiView_onStartNextWaveRequest, this, std::placeholders::_1));
}

void GPPLH_PlayLoop::uiView_onStartNextWaveRequest(IMainInGameUIView* target) {
	isNextWaveStartRequested = true;
}

void GPPLH_PlayLoop::onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event) {
	int newStatus = tickStatus(event.deltaS);
	if (newStatus != status) {
		status = newStatus;
		isStatusInit = false;
	} else if(!isStatusInit) {
		isStatusInit = true;
	}
}

int GPPLH_PlayLoop::tickStatus(float deltaS) {
	switch (status) {
	case Status::ZERO: {
        if (!isStatusInit) {
            //void
        }

        return Status::preStartDelay;
        //break;
    }
	case Status::preStartDelay: {
		if (!isStatusInit) {
			// This deltay feels off, like a bad lag.
			cooldownS = 0.0f;

			handleTurretsShopPlatforms();
		}

		cooldownS -= deltaS;
		if (cooldownS <= 0.0f) {
			return Status::waitingForNextWaveStartRequest;
		}

		break;
    }
	case Status::waitingForNextWaveStartRequest: {
		if (!isStatusInit) {
			isNextWaveStartRequested = false;

			mainUIVIew->setIsWaveStarterBtnActive(true);
		}

		if (isNextWaveStartRequested) {
			mainUIVIew->setIsWaveStarterBtnActive(false);
			
			return Status::spawningWaveEnemies;
		}

		break;
    }
	case Status::spawningWaveEnemies: {
        if (!isStatusInit) {
			cooldownS = 0.0f;
			enemiesToSpawn = 5;
			mobsActiveoOnWorld = 0;
        }

		cooldownS -= deltaS;
		if (cooldownS <= 0.0f) {
			cooldownS = 3.0f;
			enemiesToSpawn--;
			mobsActiveoOnWorld++;

			if (!isEnemySpawnLocationInit) {
				// Fetch the spawn location.
				IWorldEntity* spawnPlatform = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::mobsSpawner);
				if (spawnPlatform == nullptr) {
					throw LogicException(LOC);
				}

				enemiesSpawnLocation.set(spawnPlatform->getPos());
				enemiesSpawnRotZ = spawnPlatform->getRot()->getAngleAround(Vector3::Z);
				isEnemySpawnLocationInit = true;
			}

			// Spawn an enemy.
			IWorldEntity* entity = spawnMob(
				enemiesSpawnLocation.x/*x*/,
				enemiesSpawnLocation.y/*y*/,
				enemiesSpawnLocation.z/*z*/,
				enemiesSpawnRotZ/*rotZ*/,
				Template_Mob_General_A_V1::TEMPLATE/*entityTemplate*/,
				EntityIds::gridMob/*instanceId*/,
				gw/*gw*/
			);

			//asdA40_t_r;
			Vector2 v{-1.0f, 0.0f};
			IMovingEntityModule* movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
			movingEntity->addAction_ActionSetDirectionalMovementContinuous(
				IMovingEntityModule::ControlType::Global_Dir, 
				v, 
				true, false
			);

			// Hook a listener on the entity.
			entity->addEventListener(
				IWorldEntity::EVENT_onRemoved_pre->type,
				std::bind(&GPPLH_PlayLoop::onSpawnedWorldMobEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
				autoListenersList
			);

			if (enemiesToSpawn <= 0) {
				return Status::waitingForWaveEnd;
			}
		}

        break;
    }
	case Status::waitingForWaveEnd: {
        if (!isStatusInit) {
			//void
        }

		if (mobsActiveoOnWorld <= 0) {
			return Status::postWaveDelay;
		}

        break;
    }
	case Status::postWaveDelay: {
		if (!isStatusInit) {
			// This deltay feels off, like a bad lag.
			cooldownS = 0.0f;

			handleTurretsShopPlatforms();
		}

		cooldownS -= deltaS;
		if (cooldownS <= 0.0f) {
			return Status::waitingForNextWaveStartRequest;
		}

		break;
	}
	default: {
        assert(false);

        break;
    }}

	return status;
}

IWorldEntity* GPPLH_PlayLoop::spawnMob(
	float x, float y, float z,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId,
	base::IGameWorld* gw)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId/*instanceId*/,
		0/*ownerPlayerId*/,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[](IWorldEntity* entity) {
			rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			mob_team->setTeamIndex(TeamIndexesList::MOB);

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			mobUnit::LB_Turret_CubesCannon_A* lB_Turret_CubesCannon_A = new mobUnit::LB_Turret_CubesCannon_A();
			entity_controller->registerLogicBlock(lB_Turret_CubesCannon_A, mobUnit::ILB_Turret_CubesCannon_A::ID, static_cast<mobUnit::ILB_Turret_CubesCannon_A*>(lB_Turret_CubesCannon_A));
			entity_controller->setSingleActiveGroupLogicBlock(mobUnit::LB_Turret_CubesCannon_A::GroupID, mobUnit::LB_Turret_CubesCannon_A::ID);
		}
	);

	return entity;
}

void GPPLH_PlayLoop::handleTurretsShopPlatforms() {
	// Despawn old and spawn new turrets on the turrets platforms.
	if (turretSpawnPlatform_1 == nullptr) {
		turretSpawnPlatform_1 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::turretSpawner_1);
		turretSpawnPlatform_2 = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::turretSpawner_2);
		if (turretSpawnPlatform_1 == nullptr || turretSpawnPlatform_2 == nullptr) {
			throw LogicException(LOC);
		}
		turretSpawnPlatform_1_stackable = turretSpawnPlatform_1->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
		turretSpawnPlatform_2_stackable = turretSpawnPlatform_2->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
	}

	if (turretSpawnPlatform_1_stackable->getIsCarrying()) {
		IWorldEntity* e = turretSpawnPlatform_1_stackable->getCarrying()->getEntity();
		turretSpawnPlatform_1_stackable->setCarrying(nullptr);

		e->markEntityForRemoval(ENT_REM_REASON_PURPOSE_FINISHED, false);
	}
	if (turretSpawnPlatform_2_stackable->getIsCarrying()) {
		IWorldEntity* e = turretSpawnPlatform_2_stackable->getCarrying()->getEntity();
		turretSpawnPlatform_2_stackable->setCarrying(nullptr);

		e->markEntityForRemoval(ENT_REM_REASON_PURPOSE_FINISHED, false);
	}

	// Create a blocker and a turret.
	Vector3 spawnPos{};
	Quaternion rot{};

	spawnPos.set(turretSpawnPlatform_1->getPos());
	rot.idt();
	IWorldEntity* e = gw->getGPMain()->createEntity(
		Template_BlockerPillar_A_V1::TEMPLATE,
		spawnPos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr/*instanceId*/,
		0/*ownerPlayerId*/,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[](IWorldEntity* entity) {
			rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			mob_team->setTeamIndex(TeamIndexesList::PLAYER_UNIT);
		}
	);
	rpg3D::IStackableModule* e_stackable = e->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
	turretSpawnPlatform_1_stackable->setCarrying(e_stackable);
	
	base::IGameWorld* gw = getGW();

	spawnPos.set(turretSpawnPlatform_2->getPos());
	rot.idt();
	e = gw->getGPMain()->createEntity(
		Template_Turret_CubesCannon_A_V1::TEMPLATE,
		spawnPos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr/*instanceId*/,
		0/*ownerPlayerId*/,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[](IWorldEntity* entity) {
			rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			mob_team->setTeamIndex(TeamIndexesList::PLAYER_UNIT);

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			mobUnit::LB_Turret_CubesCannon_A* lB_Turret_CubesCannon_A = new mobUnit::LB_Turret_CubesCannon_A();
			entity_controller->registerLogicBlock(lB_Turret_CubesCannon_A, mobUnit::ILB_Turret_CubesCannon_A::ID, static_cast<mobUnit::ILB_Turret_CubesCannon_A*>(lB_Turret_CubesCannon_A));
			entity_controller->setSingleActiveGroupLogicBlock(mobUnit::LB_Turret_CubesCannon_A::GroupID, mobUnit::LB_Turret_CubesCannon_A::ID);

			ICustomCubesCannonTurretModule* customCubesCannonTurret = entity->getComponentAs<ICustomCubesCannonTurretModule*>(true/*mustExist*/);
			customCubesCannonTurret->addProjectilePayload(std::make_shared<rpg3D::EffectPayload>(
				rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f
			));
			customCubesCannonTurret->setProjectileTemplateId(Namespace::NAMESPACE, Template_Projectile_CannonCube_A_V1::ID);
		}
	);
	e_stackable = e->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
	turretSpawnPlatform_2_stackable->setCarrying(e_stackable);
}

void GPPLH_PlayLoop::onSpawnedWorldMobEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_onRemoved_pre->type) {
		if (mobsActiveoOnWorld <= 0) {
			throw LogicException(LOC);
		}

		mobsActiveoOnWorld--;

		// Handle currency gain or life losing as needed.
		if (event.entityData->getMarkedForRemovalReason() == ERR_DESTINATION_REACHED) {
			// Lose 1 life.
			if (getCurrentLives() > 0) {
				setCurrentLives(getCurrentLives() - 1);
			}
		}else if (event.entityData->getMarkedForRemovalReason() == rpg3D::LivingEntityModule::EER_LIVING_ENTITY_DESTROYED) {
			// Gain 1 gold.
			setCurrentGold(getCurrentGold() + 1);
		} else {
			// This occurs if leaving the zone.

			//void
		}
	} else {
		throw LogicException(LOC);
	}
}

GPPLH_PlayLoop::~GPPLH_PlayLoop() {
	//void
}
