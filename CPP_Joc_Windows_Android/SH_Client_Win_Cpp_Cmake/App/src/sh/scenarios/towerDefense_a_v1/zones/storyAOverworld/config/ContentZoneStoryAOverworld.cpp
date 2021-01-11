#include "ContentZoneStoryAOverworld.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/entity/TeamIndexesList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <base/statics/StaticsInit.h>
#include "../../../gw/config/ScenarioFactory.h"
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::StoryAOverworld;

std::string ContentZoneStoryAOverworld::NAME = "StoryAOverworld";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneStoryAOverworld::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneStoryAOverworld>();

	return ret;
}

ContentZoneStoryAOverworld::ContentZoneStoryAOverworld()
	: super(NAME)
{
	//void
}

void ContentZoneStoryAOverworld::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);
	
	float gridCellSize = 1.0f;

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

	Vector3 playerSpawnPos{};
	int playerSpawnPosY = 0;
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
}

void ContentZoneStoryAOverworld::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	//void
}

void ContentZoneStoryAOverworld::createGroundPatch(
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
	}else if (gridCellSize == 0.6f) {
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

void ContentZoneStoryAOverworld::createConfigEntity(
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

ContentZoneStoryAOverworld::~ContentZoneStoryAOverworld() {
	//void
}
