#include "ContentZoneMain.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include "../../../gw/config/entity/platform/Template_Platform_EnterDungeon.h"
#include <rpg3D/gw/config/entity/platform/T_Platform_E.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_P.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_T.h>
#include <base/statics/StaticsInit.h>
#include "../../../gw/config/ScenarioFactory.h"
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include "../../../gw/config/entity/EntityIds.h"
#include "../../zoneMyRoomsListA/config/ContentZoneMyRoomsListA.h"
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/npc/T_Npc_Shopkeep_A_V1.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::Main;

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
	int D = eId++;// Dungeon enter platform
	int E = eId++;// Room editor
	int P = eId++;// Prefab rooms dungeon
	int T = eId++;// Turn based random dungeon

	Array2D<int> elementsGrid(
		7, 7,
		false, true,
		new int[7 * 7] {
			O,G,G,G,G,G,D,
			G,G,G,G,G,P,T,
			G,E,G,G,G,G,G,
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
			std::shared_ptr<std::string> platformActivationScript = nullptr;
			float rotZ = 0.0f;

			if (val == D) {
				entityTemplate = Template_Platform_EnterDungeon::TEMPLATE;
				instanceId = EntityIds::enterRandomDungeonA;
			}else if (val == E) {
				// Rooms editor
				entityTemplate = rpg3D::T_Platform_E::TEMPLATE;
				platformActivationScript = std::make_shared<std::string>(
					worldGame3D::ScriptCommands::cmd_setZone + worldGame3D::ScriptCommands::commandPartsDelimiter
					+ MyRoomsListA::ContentZoneMyRoomsListA::NAME
				);
			}else if (val == P) {
				// Prefab room dungeons
				entityTemplate = rpg3D::T_Platform_P::TEMPLATE;
				instanceId = EntityIds::enterPrefabRoomsDungeonA;
			}else if (val == T) {
				// Prefab room dungeons
				entityTemplate = rpg3D::T_Platform_T::TEMPLATE;
				instanceId = EntityIds::enterTurnBasedRandomDungeonA;
			}

			if (entityTemplate != nullptr) {
				createConfigEntity(
					gx * gridCellSize,
					gy * gridCellSize,
					posZ,
					rotZ,
					entityTemplate,
					instanceId, platformActivationScript,
					gw
				);
			}
		}
	}

	IWorldEntity* homeTeleporterPlatform = createConfigEntity(1.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Platform_Teleporter_Home, nullptr/*instanceId*/, 
		std::make_shared<std::string>(
			worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
			+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id
		)/*platformActivationScript*/, gw
	);

	createConfigEntity(0.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(0.0f, 2.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(0.0f, 3.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);

	createConfigEntity(1.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(2.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(3.0f, 0.0f, 0.0f, 90.0f, rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	
	createConfigEntity(0.0f, 4.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(0.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(1.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(1.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(2.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(3.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(4.0f, 0.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(4.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(5.0f, 0.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(5.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(5.0f, 6.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(6.0f, 1.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(6.0f, 2.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(6.0f, 3.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(6.0f, 4.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	//asd_g_t; createConfigEntity(6.0f, 5.0f, 0.0f, 0.0f, rpg3D::EntityTemplatesList::Flora_TreeA, nullptr, gw);

	createConfigEntity(5.0f, 2.0f, 0.0f, 180.0f, randDungeons_a_v1::T_Npc_Shopkeep_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);
	createConfigEntity(5.0f, 3.0f, 0.0f, 180.0f, randDungeons_a_v1::T_Npc_Shopkeep_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw);

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

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
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
	base::IGameWorld* gw)
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
		[platformActivationScript](IWorldEntity* entity) -> void {
			if (platformActivationScript != nullptr) {
				entity->getComponentAs<rpg3D::IPlatformModule*>(true/*mustExist*/)->setActivation_script(platformActivationScript);
			}
		}
	);
	
	return entity;
}

void ContentZoneMain::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
}

ContentZoneMain::~ContentZoneMain() {
	//void
}
