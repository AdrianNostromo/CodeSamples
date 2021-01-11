#include "ContentZoneMain.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../ScenarioFactory.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace spaceShipMissions_a_v1;
using namespace spaceShipMissions_a_v1::Main;

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

	int eId = 0;
	int W = eId++;// Water
	int G = eId++;// Grass
	int S = eId++;// Start position platform

	Array2D<int> elementsGrid(
		2, 2,
		false, true,
		new int[2 * 2]{
			W,G,
			S,W
		}
	);

	Vector3 playerSpawnGPos{};
	int playerSpawnGPosY = 0;
	bool isPlayerPosOnGrid = false;

	for (int gx = 0; gx < elementsGrid.getLengthA(); gx++) {
		for (int gy = 0; gy < elementsGrid.getLengthB(); gy++) {
			int val = elementsGrid.getDirect(gx, gy);

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

			if (val == S) {
				playerSpawnGPos.set(gx, gy, 0.0f);
				isPlayerPosOnGrid = true;
			}
		}
	}
	if (!isPlayerPosOnGrid) {
		throw LogicException(LOC);
	}
	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnGPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnGPos);
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

ContentZoneMain::~ContentZoneMain() {
	//void
}
