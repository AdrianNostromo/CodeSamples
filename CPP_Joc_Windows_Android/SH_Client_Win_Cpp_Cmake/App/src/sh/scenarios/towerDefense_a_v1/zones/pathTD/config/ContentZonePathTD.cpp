#include "ContentZonePathTD.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include "../../../gw/config/entity/doodad/Template_Doodad_MobsSpawner.h"
#include "../../../gw/config/entity/doodad/T_Doodad_TurretSpawner.h"
#include "../../../gw/config/entity/platform/Template_Platform_MobsDespawner.h"
#include "../../../gw/config/entity/placeholder/T_Placeholder_StackableHaulerSlot.h"
#include "../../../gw/config/entity/turret/Template_Turret_CubesCannon_A_V1.h"
#include "../../../gw/config/entity/structure/Template_BlockerPillar_A_V1.h"
#include "../../../gw/config/entity/turret/Template_Turret_CubesCannon_B_V1.h"
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/TeamIndexesList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <base/statics/StaticsInit.h>
#include "../../../gw/config/ScenarioFactory.h"
#include "../gw/views/mainui/MainInGameUIView.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::PathTD;

std::string ContentZonePathTD::NAME = "PathTD";

std::shared_ptr<worldGame3D::ContentZone> ContentZonePathTD::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZonePathTD>();

	return ret;
}

ContentZonePathTD::ContentZonePathTD()
	: super(NAME)
{
	//void
}

int ContentZonePathTD::getMainUIViewID() {
	return IMainInGameUIView::ID;
}

rpg3D::MainInGameUIView* ContentZonePathTD::getNewMainUIViewUninitialised(base::IGameWorld* gw) {
	MainInGameUIView* ret = new MainInGameUIView(gw->getHandler()->getApp(), gw);

	return ret;
}

void ContentZonePathTD::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);
	
	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new GamePartPlayLogicHandler(gw));

	float gridCellSize = 0.6f;

	int eId = 0;
	int O = -1;// Nothing
	int W = eId++;// Water
	int G = eId++;// Grass
	int S = eId++;// Player spawn position
	int I = eId++;// Mobs start pad.
	int E = eId++;// Mobs end pad.
	int T = eId++;// Turret slot.
	
	int a = eId++;// TUrret spawner 1.
	int b = eId++;// TUrret spawner 2.

	Array2D<int> elementsGrid(
		25, 17,
		false, true,
		new int[25 * 17] {
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			E,G,G,G,G,G,G,T,T,T,T,T,S,T,T,T,T,T,G,G,G,G,G,G,I,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,a,W,b,W,W,W,W,W,W,W,O,O,O,O,O,
			O,O,O,O,O,W,W,W,W,W,W,W,W,W,W,W,W,W,W,W,O,O,O,O,O
		}
	);

	Vector3 playerSpawnPos{};
	int playerSpawnPosY = 0;
	bool isPlayerPosOnGrid = false;

	for (int gx = 0; gx < elementsGrid.getLengthA(); gx++) {
		for (int gy = 0; gy < elementsGrid.getLengthB(); gy++) {
			int val = elementsGrid.getDirect(gx, gy);

			if (val < 0) {
				continue;
			}

			Array3D<signed char>* voxelsGrid = rpg3D::TestVoxelStructureComponents::groundPatchGrid6x6;
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
				(-0.5f + gx) * gridCellSize,
				(-0.5f + gy) * gridCellSize,
				(posZ)*gridCellSize,
				voxelsGrid,
				materialsList,
				gw, gridCellSize
			);

			EntityTemplate* entityTemplate = nullptr;
			std::shared_ptr<std::string> instanceId = nullptr;
			float rotZ = 0.0f;
			int* teamId = nullptr;

			if (val == S) {
				playerSpawnPos.set(gx * gridCellSize, gy * gridCellSize, 0.0f * gridCellSize);
				isPlayerPosOnGrid = true;
			} else if (val == I) {
				entityTemplate = Template_Doodad_MobsSpawner::TEMPLATE;
				instanceId = EntityIds::mobsSpawner;
				rotZ = 180.0f;
			} else if (val == E) {
				entityTemplate = Template_Platform_MobsDespawner::TEMPLATE;
				instanceId = EntityIds::mobsDestination;
			} else if (val == a) {
				entityTemplate = T_Doodad_TurretSpawner::TEMPLATE;
				instanceId = EntityIds::turretSpawner_1;
			} else if (val == b) {
				entityTemplate = T_Doodad_TurretSpawner::TEMPLATE;
				instanceId = EntityIds::turretSpawner_2;
			}
			
			if (val == T || val == S) {
				entityTemplate = T_Placeholder_StackableHaulerSlot::TEMPLATE;
			}

			if (entityTemplate != nullptr) {
				createConfigEntity(
					gx * gridCellSize,
					gy * gridCellSize,
					posZ,
					rotZ,
					entityTemplate,
					instanceId,
					gw,
					teamId
				);
			}
		}
	}
	if (!isPlayerPosOnGrid) {
		throw LogicException(LOC);
	}
	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);
	
	// Set the initial gold and lives.
	IGamePartPlayLogicHandler* gpPlayLogic = gw->getComponentAs< IGamePartPlayLogicHandler*>(true/*mustExist*/);
	gpPlayLogic->setCurrentLives(5);
	gpPlayLogic->setCurrentGold(0);
}

void ContentZonePathTD::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
}

void ContentZonePathTD::createGroundPatch(
	float x, float y, float z,
	Array3D<signed char>* voxelsGrid,
	ArrayList<Visual3DModelMaterial*>* materialsList,
	base::IGameWorld* gw, float gridCellSize)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, 0.0f };
	EntityTemplate* entityTemplate;
	if (gridCellSize == 1.0f) {
		entityTemplate = rpg3D::EntityTemplatesList::Ground_BasicPatchA_1m;
	} else if (gridCellSize == 0.6f) {
		entityTemplate = rpg3D::EntityTemplatesList::Ground_BasicPatchA_6dm;
	} else {
		throw LogicException(LOC);
	}
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
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

void ContentZonePathTD::createConfigEntity(
	float x, float y, float z,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId,
	base::IGameWorld* gw,
	int* teamId)
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
		[teamId](IWorldEntity* entity) {
			if (teamId != nullptr) {
				rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				mob_team->setTeamIndex(*teamId);
			}
		}
	);
	(void)entity;
}

ContentZonePathTD::~ContentZonePathTD() {
	//void
}
