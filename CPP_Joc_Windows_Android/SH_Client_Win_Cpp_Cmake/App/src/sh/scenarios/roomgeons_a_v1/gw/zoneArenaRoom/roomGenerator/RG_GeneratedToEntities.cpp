#include "RG_GeneratedToEntities.h"
#include <base/list/ArrayList.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <base/map/ExtraData.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include "../roomContent/RoomContent.h"
#include "config/CfgRoomGenerator.h"
#include <base/object/WrappedValue.h>
#include <rpg3D/gw/config/entity/environment/TC_WallPillarA.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/math/Vector3Int.h>
#include <rpg3D/gw/config/entity/structure/TC_DoorFrame.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_LinearX.h>
#include <base/exceptions/LogicException.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RG_GeneratedToEntities::RG_GeneratedToEntities(sp<CfgRoomGenerator> config, int seed)
	: super(config, seed)
{
	//void
}

void RG_GeneratedToEntities::populateFloorsEntities() {
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

		roomContent->structureElementsList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::Floor_TiledGrid2D, tileVoxelsGridSize,
				config->tileSizeCm/*voxelAndTileSizeCM*/, 0.5f/*heightScale*/
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
		Vector3 posInRoom{ 0.0f, 0.0f, -config->tileSizeCm / 100.0f };
		Quaternion rot{ Vector3::Z, 0.0f };
		Vector3Int tileVoxelsGridSize{ 1, 1, 1 };
		roomContent->structureElementsList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::None, tileVoxelsGridSize,
				config->tileSizeCm/*voxelAndTileSizeCM*/, 10.0f/*heightScale*/
			)/*entityTemplate*/,
			posInRoom/*posInRoom*/, rot/*rot*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
			tilingInformation
		);
	}
}

void RG_GeneratedToEntities::populateWallsEntities() {
	{
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
		roomContent->structureElementsList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Wall, rpg3D::TC_GroundOrWall::PhysicsType::Wall_TiledGrid2D, tileVoxelsGridSize,
				config->tileSizeCm/*voxelAndTileSizeCM*/, 1.0f/*heightScale*/
			)/*entityTemplate*/,
			posInRoom/*posInRoom*/, rot/*rot*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
			tilingInformation
		);
	}

	{
		// Doors;
		//asd_01;// use configured size;
		EntityTemplate* door_tileableTemplate_linearX = rpg3D::TC_DoorFrame::getOrCreate(24);

		for (ListDL<Door>::Entry* entry = doorsList.getFirst(); entry != nullptr; entry = entry->next) {
			Door& doorInfo = entry->data;

			Vector3 posInRoom{};
			Quaternion rot{};

			if (doorInfo.tileSideType == TileSideType::XNeg) {
				posInRoom.set(
					doorInfo.tilesGPos.x * (config->tileSizeCm / 100.0f),
					(doorInfo.tilesGPos.y + doorInfo.tilesLength) * (config->tileSizeCm / 100.0f),
					0.0f
				);

				rot.set(Vector3::Z, -90.0f);
			} else if (doorInfo.tileSideType == TileSideType::XPos) {
				posInRoom.set(
					(doorInfo.tilesGPos.x + 1) * (config->tileSizeCm / 100.0f),
					doorInfo.tilesGPos.y * (config->tileSizeCm / 100.0f),
					0.0f
				);

				rot.set(Vector3::Z, 90.0f);
			} else if (doorInfo.tileSideType == TileSideType::YNeg) {
				posInRoom.set(
					doorInfo.tilesGPos.x * (config->tileSizeCm / 100.0f),
					doorInfo.tilesGPos.y * (config->tileSizeCm / 100.0f),
					0.0f
				);

				rot.set(Vector3::Z, 0.0f);
			} else if (doorInfo.tileSideType == TileSideType::YPos) {
				posInRoom.set(
					(doorInfo.tilesGPos.x + doorInfo.tilesLength) * (config->tileSizeCm / 100.0f),
					(doorInfo.tilesGPos.y + 1) * (config->tileSizeCm / 100.0f),
					0.0f
				);

				rot.set(Vector3::Z, 180.0f);
			} else {
				throw LogicException(LOC);
			}

			roomContent->structureElementsList.appendEmplace(
				door_tileableTemplate_linearX/*entityTemplate*/,
				posInRoom/*posInRoom*/, rot/*rot*/,
				nullptr/*materialsList*/, ""/*extraDataId_materialsList*/,
				nullptr/*tilingInformation*/
			);
		}
	}
}

RG_GeneratedToEntities::~RG_GeneratedToEntities() {
	//void
}
