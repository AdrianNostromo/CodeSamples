#include "ContentZoneMain.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../ScenarioFactory.h"
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include "../../../gw/config/entity/EntityIds.h"
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_P.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/generalActiveTool/ILBGeneralActiveTools.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <rpg3D/gw/config/entity/mob/T_Mob_General_A_V1.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::Main;

std::string ContentZoneMain::NAME = "Main";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneMain::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneMain>();

	return ret;
}

ContentZoneMain::ContentZoneMain()
	: super(NAME)
{
	//void
}

void ContentZoneMain::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new GamePartPlayLogicHandler(gw));

	float gridCellSize = 1.0f;

	int eId = 0;
	int O = -1;// Nothing
	int W = eId++;// Water
	int G = eId++;// Grass
	int P = eId++;// Play start

	Array2D<int> elementsGrid(
		7, 7,
		false, true,
		new int[7 * 7] {
			O,G,G,G,G,G,O,
			G,G,G,G,G,P,G,
			G,G,G,G,G,G,G,
			G,G,G,G,G,G,G,
			G,G,G,G,G,G,G,
			G,G,G,G,G,G,G,
			O,G,G,G,G,G,O
		}
	);

	for (int gx = 0; gx < elementsGrid.getLengthA(); gx++) {
		for (int gy = 0; gy < elementsGrid.getLengthB(); gy++) {
			int val = elementsGrid.getDirect(gx, gy);

			if (val < 0) {
				continue;
			}

			Array3D<signed char>* voxelsGrid = rpg3D::TestVoxelStructureComponents::groundPatchGrid;
			ArrayList<Visual3DModelMaterial*>* materialsList;
			float posZ;
			if (val == W) {
				posZ = -0.1f;

				materialsList = rpg3D::TestVoxelStructureComponents::waterAMaterialsList;
			} else {
				posZ = 0.0f;

				if (((gx + gy) % 2) == 0) {
					materialsList = rpg3D::TestVoxelStructureComponents::grassAMaterialsList;
				} else {
					materialsList = rpg3D::TestVoxelStructureComponents::grassBMaterialsList;
				}
			}

			createGroundPatch(
				-0.5f + gx,
				-0.5f + gy,
				posZ,
				voxelsGrid,
				materialsList,
				gw
			);

			EntityTemplate* entityTemplate = nullptr;
			std::shared_ptr<std::string> instanceId = nullptr;
			float rotZ = 0.0f;

			if (val == P) {
				// Prefab room dungeons
				entityTemplate = rpg3D::T_Platform_P::TEMPLATE;
				instanceId = EntityIds::enterGridBattleA;
			}

			if (entityTemplate != nullptr) {
				createConfigEntity(
					gx * gridCellSize,
					gy * gridCellSize,
					posZ,
					rotZ,
					entityTemplate,
					instanceId/*instanceId*/, nullptr/*platformActivationScript*/,
					gw,
					nullptr/*teamId*/
				);
			}
		}
	}

	IWorldEntity* homeTeleporterPlatform = createConfigEntity(1.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Platform_Teleporter_Home, nullptr/*instanceId*/, 
		std::make_shared<std::string>(
			worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
			+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id
		)/*platformActivationScript*/, gw, nullptr/*teamId*/);

	createConfigEntity(0.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(0.0f, 2.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(0.0f, 3.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);

	createConfigEntity(1.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(2.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(3.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	
	createConfigEntity(0.0f, 4.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(0.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(1.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(1.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(2.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(3.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(4.0f, 0.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(4.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(5.0f, 0.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(5.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(5.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(6.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(6.0f, 2.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(6.0f, 3.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	createConfigEntity(6.0f, 4.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, nullptr/*teamId*/);
	//asd_g_t; createConfigEntity(6.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr, gw, nullptr/*teamId*/);

	float projectileDamage1 = 1.0f;
	createConfigEntity(
		(3.0f + 0.5f) * gridCellSize, (3.0f + 0.5f) * gridCellSize, 0.1f, -90.0f, rpg3D::T_Mob_General_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw, &rpg3D::TeamsIndexesList::MOB/*teamId*/,
		&projectileDamage1/*projectileDamage*/
	);

	Vector3 playerSpawnPos{};
	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
		IWorldEntity* spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
		if (spawnLocationEntity == nullptr) {
			throw LogicException(LOC);
		}
		playerSpawnPos.set(spawnLocationEntity->getPos());

		if (spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(spawnLocationEntity->getInstanceId());
		}
	} else {
		playerSpawnPos.set(homeTeleporterPlatform->getPos());

		if (homeTeleporterPlatform->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(homeTeleporterPlatform->getInstanceId());
		}
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection);
	
	rpg3D::IControllerModule* entity_controller = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
	entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGeneralActiveTools::GroupID, rpg3D::playerUnit::ILBGeneralActiveTools::ID);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
	
	{
		Vector3 offsetDir{ -1.0f, -1.0f, 3.0f };
		offsetDir.nor();
		Vector3 focusPointOffset{ 0.0f, 0.0f, 0.0f };
		gw->getGPSim()->configCamera_followerThirdPersonCamera(
			offsetDir,
			4.0f, 6.0f,
			6.0f, 11.0f,
			focusPointOffset
		);
	}
}

void ContentZoneMain::createGroundPatch(
	float x, float y, float z,
	Array3D<signed char>* voxelsGrid,
	ArrayList<Visual3DModelMaterial*>* materialsList,
	base::IGameWorld* gw)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, 0.0f };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		rpg3D::EntityTemplatesList::Ground_BasicPatchA_1m,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr,
		0,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr
	);

	IVoxelisedStructureModule* voxelisedStructure = entity->getComponentAs<IVoxelisedStructureModule*>(true/*mustExist*/);
	voxelisedStructure->setStructure(
		voxelsGrid,
		materialsList
	);
}

IWorldEntity* ContentZoneMain::createConfigEntity(
	float x, float y, float z,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
	base::IGameWorld* gw,
	const int* teamId, float* projectileDamage)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId,
		0,
		nullptr,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[platformActivationScript, teamId, projectileDamage](IWorldEntity* entity) {
			if (platformActivationScript != nullptr) {
				entity->getComponentAs<rpg3D::IPlatformModule*>(true/*mustExist*/)->setActivation_script(platformActivationScript);
			}

			if (teamId != nullptr) {
				rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				mob_team->setTeamIndex(*teamId);
			}
			if (projectileDamage != nullptr) {
				entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayload(
					std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, *projectileDamage)
				);
			}
		}
	);
	
	return entity;
}

void ContentZoneMain::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	rpg3D::IControllerModule* entity_controller = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
	entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGeneralActiveTools::GroupID, -1);

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
}

ContentZoneMain::~ContentZoneMain() {
	//void
}
