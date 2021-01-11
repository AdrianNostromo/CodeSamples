#include "ContentZoneTown.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../ScenarioFactory.h"
#include "../../../gw/config/entity/EntityIds.h"
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_P.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_X.h>
#include <rpg3D/gw/config/entity/mineral/T_StoneRock_A_V1.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/ToolMelleArea.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/zoneFlags/OGPZoneFlags.h>
#include <rpg3D/gw/mobsSpawnSystem/OGPMobsSpawnSystem.h>
#include <rpg3D/gw/conditionalEntities/OGPConditionalEntities.h>
#include <base/memory/SharedPointer.h>
#include <base/container/flags/ManagedFlags.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <sh/scenarios/roomgeons_a_v1/gw/zoneTown/gw/config/entity/doodad/TC_Doodad_RespawnPlatform.h>
#include <sh/scenarios/roomgeons_a_v1/gw/zoneArenaRoom/config/ContentZoneArenaRoom.h>
#include <rpg3D/gw/config/entity/platform/TC_TeleporterPlatform_Up.h>
#include <rpg3D/gw/config/entity/platform/TC_BrokenTeleporterPlatform_Up.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::Town;

std::string ContentZoneTown::NAME = "Town";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneTown::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneTown>();

	return ret;
}

ContentZoneTown::ContentZoneTown()
	: super(NAME)
{
	//void
}

void ContentZoneTown::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	GlobalAccessLogHandler::devLogHandler->post_debug("RoomGeons Town zone activated");

	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new rpg3D::OGPZoneFlags(gw));

	gw->addGamePartUninitialised(new rpg3D::OGPMobsSpawnSystem(gw));

	gw->addGamePartUninitialised(new rpg3D::OGPConditionalEntities(gw));

	signed char typeConter = 0;
	signed char O = typeConter++;// empty;
	signed char F = typeConter++;// floor;
	signed char W = typeConter++;// wall;
	signed char H = typeConter++;// home teleporter;
	signed char R = typeConter++;// respawn location;
	signed char P = typeConter++;// start play;

	Array2D<signed char> elementsGrid(
		11, 11,
		false, true,
		new signed char[11 * 11]{
			O,W,W,W,W,W,W,W,W,W,O,//0
			W,F,F,F,F,F,F,F,F,F,W,
			W,F,F,F,F,F,F,F,P,F,W,
			W,F,F,F,F,F,F,F,F,F,W,
			W,F,F,F,F,F,F,F,F,F,W,
			W,F,F,F,F,R,F,F,F,F,W,//5
			W,F,F,F,F,F,F,F,F,F,W,
			W,F,F,F,F,F,F,F,F,F,W,
			W,F,H,F,F,F,F,F,F,F,W,
			W,F,F,F,F,F,F,F,F,F,W,
			O,W,W,W,W,W,W,W,W,W,O //10
		}
	);

	floorTilingGrid = std::make_shared<Array2D<signed char>>(elementsGrid.getLengthA(), elementsGrid.getLengthB());
	floorTilingGrid->memSetData(-1);
	wallsTilingGrid = std::make_shared<Array2D<signed char>>(elementsGrid.getLengthA(), elementsGrid.getLengthB());
	wallsTilingGrid->memSetData(-1);

	for (int gX = 0; gX < elementsGrid.getLengthA(); gX++) {
		for (int gY = 0; gY < elementsGrid.getLengthB(); gY++) {
			signed char gCell = elementsGrid.getDirect(gX, gY);

			Vector3 pos{
				(gX + 0.5f) * (tileSizeCm / 100.0f),
				(gY + 0.5f)* (tileSizeCm / 100.0f),
				0.0f
			};
			Quaternion rot{ Vector3::Z, 0.0f };

			if (gCell == O) {// Empty
				//void
			}else if (gCell == F) {// Floor
				floorTilingGrid->setDirect(gX, gY, (gX + gY) % 2);
			}else if (gCell == W) {// Wall (and floor)
				floorTilingGrid->setDirect(gX, gY, (gX + gY) % 2);

				wallsTilingGrid->setDirect(gX, gY, (gX + gY) % 2);
			}else if (gCell == H) {// Home teleporter (and floor)
				floorTilingGrid->setDirect(gX, gY, (gX + gY) % 2);

				createConfigEntity(
					pos, rot,
					rpg3D::EntityTemplatesList::Platform_Teleporter_Home,
					EntityIds::homeTeleporter/*instanceId*/,
					std::make_shared<std::string>(
						worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
						+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id
					)/*platformActivationScript*/,
					gw
				);
			}else if (gCell == R) {// Respawn location platform (and floor)
				floorTilingGrid->setDirect(gX, gY, (gX + gY) % 2);

				createConfigEntity(
					pos, rot,
					TC_Doodad_RespawnPlatform::getOrCreateTemplate(10/*voxelSizeCM*/),
					EntityIds::respawnLocationPlatform/*instanceId*/,
					nullptr/*platformActivationScript*/, gw
				);
			}else if (gCell == P) {// Play start platform (and floor)
				floorTilingGrid->setDirect(gX, gY, (gX + gY) % 2);

				createConfigEntity(
					pos, rot,
					rpg3D::TC_TeleporterPlatform_Up::getOrCreateTemplate(10/*voxelSizeCM*/),
					nullptr/*instanceId*/,
					std::make_shared<std::string>(
						worldGame3D::ScriptCommands::cmd_setZone + worldGame3D::ScriptCommands::commandPartsDelimiter
						+ ArenaRoom::ContentZoneArenaRoom::NAME
					)/*platformActivationScript*/, gw
				);
			} else {
				throw LogicException(LOC);
			}
		}
	}

	// Create floor and walls;
	populateFloorsEntities();
	populateWallsEntities();

	for (ListDL<StructureElement>::Entry* entry = structureElementsList.getFirst(); entry != nullptr; entry = entry->next) {
		StructureElement& entityCreateInfo = entry->data;

		std::shared_ptr<ExtraData> extraData = nullptr;

		if (entityCreateInfo.materialsList != nullptr) {
			if (extraData == nullptr) {
				extraData = std::make_shared<ExtraData>();
			}

			extraData->setData(entityCreateInfo.extraDataId_materialsList, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(entityCreateInfo.materialsList, -1));
		}

		if (entityCreateInfo.tilingInformation != nullptr) {
			if (extraData == nullptr) {
				extraData = std::make_shared<ExtraData>();
			}

			extraData->setData(EntityTemplate::ExtraDataID_tilingInformation, base::IWrappedValue::new_CustomB<std::shared_ptr<graphics::TilingInformation>>(entityCreateInfo.tilingInformation, "graphics::TilingInformation"));
		}

		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityCreateInfo.entityTemplate,
			entityCreateInfo.posInRoom,
			&entityCreateInfo.rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr,
			0,
			extraData/*extraData*/,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			nullptr
		);
	}

	// This is used if playerSpawnTargetPlatform is nullptr.
	IWorldEntity* playerSpawnTargetPlatform = nullptr;
	Vector3 playerSpawnPos{};
	bool isPlayerSpawnPosSet = false;

	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
		playerSpawnTargetPlatform = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
		if (playerSpawnTargetPlatform == nullptr) {
			throw LogicException(LOC);
		}
	} else {
		IWorldEntity* homeTeleporterPlatform = gw->getGPMain()->getEntityWithInstanceId(*EntityIds::homeTeleporter);
		if (homeTeleporterPlatform == nullptr) {
			// At first spawn, must use the home teleporter.
			throw LogicException(LOC);
		}

		playerSpawnTargetPlatform = homeTeleporterPlatform;
	}
	//playerSpawnTargetPlatform may be nullptr if it is a conditional entity. playerSpawnPos should have a correct value.
	if (!isPlayerSpawnPosSet) {
		if (playerSpawnTargetPlatform == nullptr) {
			throw LogicException(LOC);
		}

		playerSpawnPos.set(playerSpawnTargetPlatform->getPos());
	}

	if (playerSpawnTargetPlatform != nullptr) {
		if (playerSpawnTargetPlatform->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(playerSpawnTargetPlatform->getInstanceId());
		}
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	// Don't heal the playter.
	// livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);

	Color col{ "#64B0FFFF" };
	GLUtil::gl->setCachedClearColor(col);
}

void ContentZoneTown::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	gw->disposeGamePart<rpg3D::IOGPConditionalEntities*>();
	gw->disposeGamePart<rpg3D::IOGPMobsSpawnSystem*>();
	gw->disposeGamePart<rpg3D::IOGPZoneFlags*>();
}

void ContentZoneTown::createConfigEntity(
	Vector3& pos, Quaternion& rot,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
	base::IGameWorld* gw)
{
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
	(void)entity;
}

void ContentZoneTown::populateFloorsEntities() {
	// Create tiling information and a floor entity.
	{
		// Top grass layer.
		std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
		tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassADirtA());
		tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassBDirtA());

		std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
			rpg3D::TC_GroundOrWall::TileVoxelsCount_2Layers,
			VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
			tilesConfigList,
			floorTilingGrid,
			nullptr/*gridGeometryOffset*/
		);

		Vector3 posInRoom{ 0.0f, 0.0f, 0.0f };
		Quaternion rot{ Vector3::Z, 0.0f };
		Vector3Int tileVoxelsGridSize{ 1, 1, 2 };

		structureElementsList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::Floor_TiledGrid2D, tileVoxelsGridSize,
				tileSizeCm/*voxelAndTileSizeCM*/, 0.5f/*heightScale*/
			)/*entityTemplate*/,
			posInRoom/*posInRoom*/, rot/*rot*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
			tilingInformation
		);
	}

	{
		// The under layer with dirt and stone.
		int eId = 0;
		int O = -1;// Nothing
		signed char A = eId++;

		std::shared_ptr<Array2D<signed char>>tilingGridStone = std::make_shared<Array2D<signed char>>(
			floorTilingGrid->getLengthA(), floorTilingGrid->getLengthB()
			);
		for (int gX = 0; gX < floorTilingGrid->getLengthA(); gX++) {
			for (int gY = 0; gY < floorTilingGrid->getLengthB(); gY++) {
				if (floorTilingGrid->getDirect(gX, gY) >= 0) {
					tilingGridStone->setDirect(gX, gY, A);
				} else {
					tilingGridStone->setDirect(gX, gY, O);
				}
			}
		}

		std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
		tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneA());

		std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
			rpg3D::TC_GroundOrWall::TileVoxelsCount_1Layer,
			VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin | VoxelSides::BitGroup_zMax*//*enabledSidesTriangulationBitMask*/,
			tilesConfigList,
			tilingGridStone,
			nullptr/*gridGeometryOffset*/
		);

		// The z pos (-(config->tileSizeCm / 100.0f)) if the height of the grass_and_dirt layer template used above with a scale_z of 0.5f.
		Vector3 posInRoom{ 0.0f, 0.0f, -tileSizeCm / 100.0f };
		Quaternion rot{ Vector3::Z, 0.0f };
		Vector3Int tileVoxelsGridSize{ 1, 1, 1 };
		structureElementsList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::None, tileVoxelsGridSize,
				tileSizeCm/*voxelAndTileSizeCM*/, 10.0f/*heightScale*/
			)/*entityTemplate*/,
			posInRoom/*posInRoom*/, rot/*rot*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
			tilingInformation
		);
	}
}

void ContentZoneTown::populateWallsEntities() {
	// Walls tiled grid2D single entity
	std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
	tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneC());
	tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneD());

	std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
		rpg3D::TC_GroundOrWall::TileVoxelsCount_1Layer,
		VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
		tilesConfigList,
		wallsTilingGrid,
		nullptr/*gridGeometryOffset*/
	);

	Vector3 posInRoom{ 0.0f, 0.0f, 0.0f };
	Quaternion rot{ Vector3::Z, 0.0f };
	Vector3Int tileVoxelsGridSize{ 1, 1, 1 };

	structureElementsList.appendEmplace(
		rpg3D::TC_GroundOrWall::getOrCreateTemplate(
			rpg3D::TC_GroundOrWall::UseType::Wall, rpg3D::TC_GroundOrWall::PhysicsType::Wall_TiledGrid2D, tileVoxelsGridSize,
			tileSizeCm/*voxelAndTileSizeCM*/, 1.0f/*heightScale*/
		)/*entityTemplate*/,
		posInRoom/*posInRoom*/, rot/*rot*/,
		rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
		tilingInformation
	);
}

ContentZoneTown::~ContentZoneTown() {
	//void
}
