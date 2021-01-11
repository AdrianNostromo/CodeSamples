#include "ContentZoneMain.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include "../entity/flora/Template_Flora_BerryBushA_V1.h"
#include "../entity/mob/Template_Mob_ChickenA_V1.h"
#include <base/statics/StaticsInit.h>
#include "../ScenarioFactory.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace islandSurvival_a_v1;
using namespace islandSurvival_a_v1::Main;

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

	float gridCellSize = 1.0f;

	int eId = 0;
	int W = eId++;// Water
	int T = eId++;// Tree
	int D = eId++;// Deep Water
	int G = eId++;// Grass
	int S = eId++;// Start position platform

	Array2D<int> elementsGrid(
		20, 20,
		false, true,
		new int[20 * 20]{
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,
			D,D,D,W,W,W,W,W,W,W,W,W,W,W,W,W,W,D,D,D,
			D,D,D,W,W,W,W,W,W,W,W,W,W,W,W,W,W,D,D,D,
			D,D,D,W,W,T,G,G,G,G,G,G,G,G,T,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,G,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,G,S,G,G,G,G,G,G,G,G,W,W,D,D,D,
			D,D,D,W,W,T,G,G,G,G,G,G,G,G,T,W,W,D,D,D,
			D,D,D,W,W,W,W,W,W,W,W,W,W,W,W,W,W,D,D,D,
			D,D,D,W,W,W,W,W,W,W,W,W,W,W,W,W,W,D,D,D,
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,
			D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D,D
		}
	);

	Vector3 playerSpawnPos{};
	int playerSpawnGPosY = 0;
	bool isPlayerPosOnGrid = false;

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
			} else if (val == D) {
				// Deep water
				posZ = -0.1f;

				materialsList = rpg3D::TestVoxelStructureComponents::deepWaterAMaterialsList;
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

			if (val == T) {
				entityTemplate = rpg3D::EntityTemplatesList::Flora_TreeA;
			} else if (val == S) {
				playerSpawnPos.set(gx * gridCellSize, gy * gridCellSize, 0.0f * gridCellSize);
				isPlayerPosOnGrid = true;
			}

			if (entityTemplate != nullptr) {
				createConfigEntity(
					gx * gridCellSize,
					gy * gridCellSize,
					posZ,
					rotZ,
					entityTemplate,
					instanceId,
					gw
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

	// Spawn berry bushes.
	createConfigEntity(
		playerSpawnPos.x + 3.0f,
		playerSpawnPos.y + 3.0f,
		0.0f,
		0.0f,
		Template_Flora_BerryBushA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 4.0f,
		playerSpawnPos.y + 4.0f,
		0.0f,
		0.0f,
		Template_Flora_BerryBushA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 5.0f,
		playerSpawnPos.y + 5.0f,
		0.0f,
		0.0f,
		Template_Flora_BerryBushA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 6.0f,
		playerSpawnPos.y + 6.0f,
		0.0f,
		0.0f,
		Template_Flora_BerryBushA_V1::TEMPLATE,
		nullptr,
		gw
	);

	// Spawn chicken mobs.
	createConfigEntity(
		playerSpawnPos.x + 3.0f,
		playerSpawnPos.y + 0.0f,
		0.0f,
		0.0f,
		Template_Mob_ChickenA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 4.0f,
		playerSpawnPos.y + 0.0f,
		0.0f,
		0.0f,
		Template_Mob_ChickenA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 5.0f,
		playerSpawnPos.y + 0.0f,
		0.0f,
		0.0f,
		Template_Mob_ChickenA_V1::TEMPLATE,
		nullptr,
		gw
	);
	createConfigEntity(
		playerSpawnPos.x + 6.0f/*x*/,
		playerSpawnPos.y + 0.0f/*y*/,
		0.0f/*z*/,
		0.0f/*rotZ*/,
		Template_Mob_ChickenA_V1::TEMPLATE/*entityTemplate*/,
		nullptr/*instanceId*/,
		gw/*gw*/
	);
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

void ContentZoneMain::createConfigEntity(
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
		instanceId,
		0,
		nullptr,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr
	);
	(void)entity;
}

ContentZoneMain::~ContentZoneMain() {
	//void
}
