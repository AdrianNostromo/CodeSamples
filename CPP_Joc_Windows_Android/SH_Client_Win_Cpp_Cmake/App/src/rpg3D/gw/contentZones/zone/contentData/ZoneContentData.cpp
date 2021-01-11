#include "ZoneContentData.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/flags/IFlags.h>
#include <base/map/ExtraData.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <functional>
#include <rpg3D/gw/config/entity/EntityIds.h>
#include <sstream>
#include <base/util/StringUtil.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <worldGame3D/gw/util/WrappedWorldFlag.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include "sh/scenarios/squareTactics_a_v1/gw/config/entity/EntityIds.h"

using namespace rpg3D;

ZoneContentData::ZoneContentData() {
	//void
}

void ZoneContentData::createContent(base::IGameWorld* gw) {
	// Set this here.
	gw->getFlags().set(EntityIds::gwFlagName_zoneEditorTileCellSizeCm, base::IWrappedValue::new_int(tileCellSizeCm), true/*isZoneOwnedFlag*/);

	gw->getFlags().set(EntityIds::gwFlagName_zoneEditorAssetInstanceIdCounter, base::IWrappedValue::new_int(zoneEditorAssetInstanceIdCounter), true/*isZoneOwnedFlag*/);

	for (ListDL<EntitiesCreateInfo>::Entry* entry = entitiesInfosList.getFirst(); entry != nullptr; entry = entry->next) {
		EntitiesCreateInfo& entityCreateInfo = entry->data;

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

		std::shared_ptr<std::string> platformActivationScript = entityCreateInfo.platformActivationScript;
		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityCreateInfo.entityTemplate,
			entityCreateInfo.posInRoom,
			&entityCreateInfo.rot/*rot*/, nullptr/*cardinalRotation*/,
			entityCreateInfo.instanceId/*instanceId*/,
			0,
			extraData/*extraData*/,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[platformActivationScript](IWorldEntity* entity) -> void {
				if (platformActivationScript != nullptr) {
					entity->getComponentAs<rpg3D::IPlatformModule*>(true/*mustExist*/)->setActivation_script(platformActivationScript);
				}
			}/*entityPostInitCb*/
		);
	}
}

std::shared_ptr<std::stringstream> ZoneContentData::Serialise(base::IGameWorld* gw) {
	std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
	*ss << std::setprecision(7);

	sp<worldGame3D::WrappedWorldFlag> wWFlag_zoneEditorAssetInstanceIdCounter = gw->getFlags().get(EntityIds::gwFlagName_zoneEditorAssetInstanceIdCounter, false/*mustExist*/);
	int zoneEditorAssetInstanceIdCounter = 0;
	if (wWFlag_zoneEditorAssetInstanceIdCounter != nullptr) {
		zoneEditorAssetInstanceIdCounter = wWFlag_zoneEditorAssetInstanceIdCounter->worldFlag->wValue->getReferenceAs_int();
	}
	*ss << "zoneEditorAssetInstanceIdCounter " << zoneEditorAssetInstanceIdCounter << "\n";

	//asd_01;// implement more info like version, sun_light infos.

	ArrayList<IWorldEntity*>* entitiesList = gw->getGPMain()->peekEntitiesList();
	for (int i = 0; i < entitiesList->count(); i++) {
		IWorldEntity* entity = entitiesList->getDirect(i);

		std::shared_ptr<std::string> instanceId = entity->getInstanceId();

		if (instanceId == nullptr || instanceId == rpg3D::EntityIds::playerUnit) {
			// Entity not saved.
			continue;
		}

		Vector3* pos = entity->getPos();
		float yawZDeg, pitchYDeg, rollXDeg;
		entity->getRot()->getEulerAnglesZYXDeg(yawZDeg, pitchYDeg, rollXDeg);

		if (i > 0) {
			*ss << "\n";
		}
		*ss << "entity\n";
		*ss << "templateInfo ";
		entity->getTemplate()->toPartsString(*ss);
		*ss << "\n";
		*ss << "instanceId " << *instanceId << "\n";
		*ss << "pos " << pos->x << " " << pos->y << " " << pos->z << "\n";
		*ss << "rotEulerZYXDeg " << yawZDeg << " " << pitchYDeg << " " << rollXDeg << "\n";

		rpg3D::IPlatformModule* platform = entity->getComponentAs<rpg3D::IPlatformModule*>(false/*mustExist*/);
		if (platform != nullptr && platform->getActivation_script() != nullptr) {
			*ss << "platformActivationScript " << *platform->getActivation_script() << "\n";
		}

		if (instanceId == rpg3D::EntityIds::groundTiledXY) {
			worldGame3D::IVisual3DModule* entity_visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
			VoxelGridDrawable3D* entity_visual3D_editingVoxelsVisual = entity_visual3D->getDrawableMustExistAs<VoxelGridDrawable3D*>("_VoxelGridGroup_");

			Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->peekGrid();
			int gCountX = voxelsGrid->getLengthA();
			int gCountY = voxelsGrid->getLengthB();
			int gCountZ = voxelsGrid->getLengthC();

			if (gCountZ != 2) {
				// Currently the top layer must be 2 tiles high;
				throw LogicException(LOC);
			}

			Vector3* voxelGridOffset = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

			*ss << "tilingGrid ";
			voxelGridOffset->toString(*ss, true/*includeTypePrefix*/);
			*ss << " " << gCountX << " " << gCountY;

			std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(
				gCountX/*_lengthA*/, gCountY/*_lengthB*/
			);
			for (int gX = 0; gX < gCountX; gX++) {
				for (int gY = 0; gY < gCountY; gY++) {
					// Grab the top most voxel matIndex;
					signed char matIndex = voxelsGrid->getReference(gX, gY, gCountZ - 1).matIndex;

					// Use int or the ss will write the actual byte and not the number;
					int tileIndex = -1;
					if (matIndex == rpg3D::TC_GroundOrWall::MaterialIndices::GrassA) {
						tileIndex = 0;
					} else if (matIndex == rpg3D::TC_GroundOrWall::MaterialIndices::GrassB) {
						tileIndex = 1;
					} else if (matIndex == -1) {
						tileIndex = -1;
					} else {
						throw LogicException(LOC);
					}

					if (gX == 0 && gY == 0) {
						*ss << " " << tileIndex;
					} else {
						*ss << ";" << tileIndex;
					}
				}
			}

			*ss << "\n";
		}else if (instanceId == rpg3D::EntityIds::underGroundTiledXY) {
			worldGame3D::IVisual3DModule* entity_visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
			VoxelGridDrawable3D* entity_visual3D_editingVoxelsVisual = entity_visual3D->getDrawableMustExistAs<VoxelGridDrawable3D*>("_VoxelGridGroup_");

			Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->peekGrid();
			int gCountX = voxelsGrid->getLengthA();
			int gCountY = voxelsGrid->getLengthB();
			int gCountZ = voxelsGrid->getLengthC();

			if (gCountZ != 1) {
				// Currently the top layer must be 2 tiles high;
				throw LogicException(LOC);
			}

			Vector3* voxelGridOffset = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

			*ss << "tilingGrid ";
			voxelGridOffset->toString(*ss, true/*includeTypePrefix*/);
			*ss << " " << gCountX << " " << gCountY;

			std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(
				gCountX/*_lengthA*/, gCountY/*_lengthB*/
			);
			for (int gX = 0; gX < gCountX; gX++) {
				for (int gY = 0; gY < gCountY; gY++) {
					// Grab the top most voxel matIndex;
					signed char matIndex = voxelsGrid->getReference(gX, gY, gCountZ - 1).matIndex;

					// Use int or the ss will write the actual byte and not the number;
					int tileIndex = -1;
					if (matIndex == rpg3D::TC_GroundOrWall::MaterialIndices::StoneA) {
						tileIndex = 0;
					} else if (matIndex == -1) {
						tileIndex = -1;
					} else {
						throw LogicException(LOC);
					}

					if (gX == 0 && gY == 0) {
						*ss << " " << tileIndex;
					} else {
						*ss << ";" << tileIndex;
					}
				}
			}

			*ss << "\n";
		}else if (instanceId == rpg3D::EntityIds::wallsFullTileSizeTiledXY) {
			worldGame3D::IVisual3DModule* entity_visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
			VoxelGridDrawable3D* entity_visual3D_editingVoxelsVisual = entity_visual3D->getDrawableMustExistAs<VoxelGridDrawable3D*>("_VoxelGridGroup_");

			Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->peekGrid();
			int gCountX = voxelsGrid->getLengthA();
			int gCountY = voxelsGrid->getLengthB();
			int gCountZ = voxelsGrid->getLengthC();

			if (gCountZ != 1) {
				// Currently the top layer must be 2 tiles high;
				throw LogicException(LOC);
			}

			Vector3* voxelGridOffset = entity_visual3D_editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

			*ss << "tilingGrid ";
			voxelGridOffset->toString(*ss, true/*includeTypePrefix*/);
			*ss << " " << gCountX << " " << gCountY;

			std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(
				gCountX/*_lengthA*/, gCountY/*_lengthB*/
			);
			for (int gX = 0; gX < gCountX; gX++) {
				for (int gY = 0; gY < gCountY; gY++) {
					// Grab the top most voxel matIndex;
					signed char matIndex = voxelsGrid->getReference(gX, gY, gCountZ - 1).matIndex;

					// Use int or the ss will write the actual byte and not the number;
					int tileIndex = -1;
					if (matIndex == rpg3D::TC_GroundOrWall::MaterialIndices::StoneC) {
						tileIndex = 0;
					} else if (matIndex == rpg3D::TC_GroundOrWall::MaterialIndices::StoneD) {
						tileIndex = 1;
					} else if (matIndex == -1) {
						tileIndex = -1;
					} else {
						throw LogicException(LOC);
					}

					// Use ";" for separation to avoid large un-needed splits(" ");
					if (gX == 0 && gY == 0) {
						*ss << " " << tileIndex;
					} else {
						*ss << ";" << tileIndex;
					}
				}
			}

			*ss << "\n";
		}
	}

	return ss;
}

sp<ZoneContentData> ZoneContentData::Parse(std::string& linesString) {
	sp<ZoneContentData> zoneContentData = new ZoneContentData();

	std::shared_ptr<ArrayList<std::string>> linesList = StringUtil::splitToLines(linesString);
	int cLineIndex = 0;

	{
		std::string& line = linesList->getReference(cLineIndex++);
		StringUtil::parsePropertyValueI(line, "zoneEditorAssetInstanceIdCounter", " ", &zoneContentData->zoneEditorAssetInstanceIdCounter);
	}

	int linesCount = linesList->count();
	while (cLineIndex < linesCount) {
		// Grab a entire entity in each of these loops.
		{
			std::string& line = linesList->getReference(cLineIndex++);
			if (line != "entity") {
				if (line.length() == 0) {
					// 0 length lines are allowed as groups delimitators.
					continue;
				}

				throw LogicException(LOC);
			}
		}

		EntityTemplate* entityTemplate = nullptr;
		{
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false);
			if (lineParts->count() < 2 || lineParts->getReference(0) != "templateInfo") {
				throw LogicException(LOC);
			}

			entityTemplate = EntityTemplate::FromPartsString(lineParts, 1/*startIndex*/);
			if (entityTemplate == nullptr) {
				throw LogicException(LOC);
			}
		}

		std::shared_ptr<std::string> instanceId = nullptr;
		{
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false);
			if (lineParts->count() != 2 || lineParts->getReference(0) != "instanceId") {
				throw LogicException(LOC);
			}
			//asd_0x; change when String & Trie are implemented // for 2 platforms, ground, wall, use the actual instanceId variables or comparissons won't work;
			if (lineParts->getReference(1) == *rpg3D::EntityIds::defaultZoneEntrance) {
				instanceId = rpg3D::EntityIds::defaultZoneEntrance;
			} else if (lineParts->getReference(1) == *rpg3D::EntityIds::gridOriginIndicator) {
				instanceId = rpg3D::EntityIds::gridOriginIndicator;
			} else if (lineParts->getReference(1) == *squareTactics_a_v1::EntityIds::enterGridBattleA) {
				instanceId = squareTactics_a_v1::EntityIds::enterGridBattleA;
			} else if (lineParts->getReference(1) == *rpg3D::EntityIds::groundTiledXY) {
				instanceId = rpg3D::EntityIds::groundTiledXY;
			} else if (lineParts->getReference(1) == *rpg3D::EntityIds::underGroundTiledXY) {
				instanceId = rpg3D::EntityIds::underGroundTiledXY;
			} else if (lineParts->getReference(1) == *rpg3D::EntityIds::wallsFullTileSizeTiledXY) {
				instanceId = rpg3D::EntityIds::wallsFullTileSizeTiledXY;
			} else {
				instanceId = std::make_shared<std::string>(lineParts->getReference(1));
			}
		}

		Vector3 pos{};
		{
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false);
			if (lineParts->count() != 4 || lineParts->getReference(0) != "pos") {
				throw LogicException(LOC);
			}

			float posX = StringUtil::parseFloat(lineParts->getReference(1));
			float posY = StringUtil::parseFloat(lineParts->getReference(2));
			float posZ = StringUtil::parseFloat(lineParts->getReference(3));

			pos.set(posX, posY, posZ);
		}

		Quaternion rot{};
		{
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false);
			if (lineParts->count() != 4 || lineParts->getReference(0) != "rotEulerZYXDeg") {
				throw LogicException(LOC);
			}

			float yawZDeg = StringUtil::parseFloat(lineParts->getReference(1));
			float pitchYDeg = StringUtil::parseFloat(lineParts->getReference(2));
			float rollXDeg = StringUtil::parseFloat(lineParts->getReference(3));

			rot.setEulerAnglesZYXDeg(yawZDeg, pitchYDeg, rollXDeg);
		}

		std::shared_ptr<std::string> platformActivationScript = nullptr;
		if (cLineIndex < linesList->count()) {
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false, 2/*maxLinesCount*/);
			if (lineParts->count() == 2 && lineParts->getReference(0) == "platformActivationScript") {
				platformActivationScript = std::make_shared<std::string>(lineParts->getReference(1));
			} else {
				cLineIndex--;
			}
		}

		ArrayList<Visual3DModelMaterial*>* materialsList = nullptr;
		std::string extraDataId_materialsList = "";
		std::shared_ptr<graphics::TilingInformation> tilingInformation = nullptr;

		if (cLineIndex < linesList->count()) {
			std::string& line = linesList->getReference(cLineIndex++);
			std::shared_ptr<ArrayList<std::string>> lineParts = StringUtil::split(line, " ", false);
			if (lineParts->count() == 5 && lineParts->getReference(0) == "tilingGrid") {
				sp<Vector3> voxelGridOffset = new Vector3{ lineParts->getReference(1) };

				int gridCountX = StringUtil::parseInt(lineParts->getReference(2));
				int gridCountY = StringUtil::parseInt(lineParts->getReference(3));

				std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(gridCountX/*_lengthA*/, gridCountY/*_lengthB*/);
				std::shared_ptr<ArrayList<std::string>> gridValsList = StringUtil::split(lineParts->getReference(4), ";", false);
				if (gridValsList->count() != gridCountX * gridCountY) {
					throw LogicException(LOC);
				}

				int cValIndex = 0;
				for (int gX = 0; gX < gridCountX; gX++) {
					for (int gY = 0; gY < gridCountY; gY++) {
						int tileIndex = StringUtil::parseInt(gridValsList->getReference(cValIndex++));

						tilingGrid->setDirect(gX, gY, tileIndex);
					}
				}

				if (instanceId == rpg3D::EntityIds::groundTiledXY) {
					// Top grass layer.
					std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
					tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassADirtA());
					tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassBDirtA());

					tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
						rpg3D::TC_GroundOrWall::TileVoxelsCount_2Layers,
						VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
						tilesConfigList,
						tilingGrid,
						voxelGridOffset
					);
				} else if (instanceId == rpg3D::EntityIds::underGroundTiledXY) {
					std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
					tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneA());

					tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
						rpg3D::TC_GroundOrWall::TileVoxelsCount_1Layer,
						VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin | VoxelSides::BitGroup_zMax*//*enabledSidesTriangulationBitMask*/,
						tilesConfigList,
						tilingGrid,
						voxelGridOffset
					);
				} else if (instanceId == rpg3D::EntityIds::wallsFullTileSizeTiledXY) {
					// Walls tiled grid2D single entity
					std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
					tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneC());
					tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneD());

					tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
						rpg3D::TC_GroundOrWall::TileVoxelsCount_1Layer,
						VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
						tilesConfigList,
						tilingGrid,
						voxelGridOffset
					);
				}
			} else {
				cLineIndex--;
			}
		}

		zoneContentData->entitiesInfosList.appendEmplace(
			entityTemplate/*entityTemplate*/,//asd_01;
			pos/*posInRoom*/, rot/*rot*/,
			instanceId/*instanceId*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials()/*materialsList*/, rpg3D::TC_GroundOrWall::DATA_KEY_materials/*extraDataId_materialsList*/,
			tilingInformation/*tilingInformation*/,
			platformActivationScript/*platformActivationScript*/
		);
	}

	return zoneContentData;
}

sp<ZoneContentData> ZoneContentData::GetNewBasicZone(
	GroundType groundType, bool createTallUnderground,
	bool createWallsTiledXY,
	bool createStarterAreaTeleporter)
{
	sp<rpg3D::ZoneContentData> zoneContentData = new rpg3D::ZoneContentData();

	int tileCellSizeCm = zoneContentData->tileCellSizeCm = 60;

	{
		signed char typeConter = -1;
		signed char O = typeConter++;// empty;
		signed char A = typeConter++;// floor a;
		signed char B = typeConter++;// floor b;

		std::shared_ptr<Array2D<signed char>> floorTilingGrid = std::make_shared<Array2D<signed char>>(
			10/*_lengthA*/, 10/*_lengthB*/,
			false/*invertA*/, true/*invertB*/,
			new signed char[10 * 10]{
				B,A,B,A,B,A,B,A,B,A,//0
				A,B,A,B,A,B,A,B,A,B,
				B,A,B,A,B,A,B,A,B,A,
				A,B,A,B,A,B,A,B,A,B,
				B,A,B,A,B,A,B,A,B,A,
				A,B,A,B,A,B,A,B,A,B,//5
				B,A,B,A,B,A,B,A,B,A,
				A,B,A,B,A,B,A,B,A,B,
				B,A,B,A,B,A,B,A,B,A,
				A,B,A,B,A,B,A,B,A,B//9
			}
		);

		{
			// Top grass layer.
			Vector3Int tileVoxelsGridSize{};

			std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
			if (groundType == Grass_1Layer) {
				tileVoxelsGridSize.set(1, 1, 1);

				tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassA());
				tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassB());
			} else if (groundType == GrassDirt_2Layers) {
				tileVoxelsGridSize.set(1, 1, 2);
				
				tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassADirtA());
				tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridGrassBDirtA());
			} else {
				throw LogicException(LOC);
			}

			std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
				rpg3D::TC_GroundOrWall::TileVoxelsCount_2Layers,
				VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
				tilesConfigList,
				floorTilingGrid,
				nullptr/*gridGeometryOffset*/
			);

			Vector3 posInRoom{ 0.0f, 0.0f, 0.0f };
			Quaternion rot{ Vector3::Z, 0.0f };

			zoneContentData->entitiesInfosList.appendEmplace(
				rpg3D::TC_GroundOrWall::getOrCreateTemplate(
					rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::Floor_TiledGrid2D, tileVoxelsGridSize /*tileVoxelsGridSize*/,
					tileCellSizeCm/*voxelAndtileCellSizeCm*/, 0.5f/*heightScale*/
				)/*entityTemplate*/,
				posInRoom/*posInRoom*/, rot/*rot*/,
				EntityIds::groundTiledXY/*instanceId*/,
				rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
				tilingInformation,
				nullptr/*platformActivationScript*/
			);
		}

		if(createTallUnderground) {
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

			// The z pos (-(config->tileCellSizeCm / 100.0f)) if the height of the grass_and_dirt layer template used above with a scale_z of 0.5f.
			Vector3 posInRoom{ 0.0f, 0.0f, -tileCellSizeCm / 100.0f };
			Quaternion rot{ Vector3::Z, 0.0f };
			Vector3Int tileVoxelsGridSize{ 1, 1, 1 };
			zoneContentData->entitiesInfosList.appendEmplace(
				rpg3D::TC_GroundOrWall::getOrCreateTemplate(
					rpg3D::TC_GroundOrWall::UseType::Floor, rpg3D::TC_GroundOrWall::PhysicsType::None, tileVoxelsGridSize,
					tileCellSizeCm/*voxelAndtileCellSizeCm*/, 10.0f/*heightScale*/
				)/*entityTemplate*/,
				posInRoom/*posInRoom*/, rot/*rot*/,
				EntityIds::underGroundTiledXY/*instanceId*/,
				rpg3D::TC_GroundOrWall::getOrCreateMaterials(), rpg3D::TC_GroundOrWall::DATA_KEY_materials,
				tilingInformation,
				nullptr/*platformActivationScript*/
			);
		}
	}

	if(createWallsTiledXY) {
		signed char typeConter = -1;
		signed char O = typeConter++;// empty;
		signed char A = typeConter++;// floor a;
		signed char B = typeConter++;// floor b;

		std::shared_ptr<Array2D<signed char>> wallsTilingGrid = std::make_shared<Array2D<signed char>>(
			10/*_lengthA*/, 10/*_lengthB*/,
			false/*invertA*/, true/*invertB*/,
			new signed char[10 * 10]{
				B,A,B,A,B,A,B,A,B,A,//0
				A,O,O,O,O,O,O,O,O,B,
				B,O,O,O,O,O,O,O,O,A,
				A,O,O,O,O,O,O,O,O,B,
				B,O,O,O,O,O,O,O,O,A,
				A,O,O,O,O,O,O,O,O,B,//5
				B,O,O,O,O,O,O,O,O,A,
				A,O,O,O,O,O,O,O,O,B,
				B,O,O,O,O,O,O,O,O,A,
				A,B,A,B,A,B,A,B,A,B//9
			}
		);

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
		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::TC_GroundOrWall::getOrCreateTemplate(
				rpg3D::TC_GroundOrWall::UseType::Wall, rpg3D::TC_GroundOrWall::PhysicsType::Wall_TiledGrid2D, tileVoxelsGridSize,
				tileCellSizeCm/*voxelAndtileCellSizeCm*/, 1.0f/*heightScale*/
			)/*entityTemplate*/,
			posInRoom/*posInRoom*/, rot/*rot*/,
			EntityIds::wallsFullTileSizeTiledXY/*instanceId*/,
			rpg3D::TC_GroundOrWall::getOrCreateMaterials()/*materialsList*/, rpg3D::TC_GroundOrWall::DATA_KEY_materials/*extraDataId_materialsList*/,
			tilingInformation/*tilingInformation*/,
			nullptr/*platformActivationScript*/
		);
	}

	if(createStarterAreaTeleporter) {
		float gridCellSizeM = tileCellSizeCm / 100.0f;

		Vector3 pos{};
		Quaternion rot{};

		zoneContentData->entitiesInfosList.appendEmplace(
			rpg3D::EntityTemplatesList::Platform_Teleporter_Home/*entityTemplate*/,
			*pos.set((0.5f + 2.0f) * gridCellSizeM, (0.5f + 2.0f) * gridCellSizeM, 0.0f)/*posInRoom*/,
			*rot.set(Vector3::Z, 0.0f)/*rot*/,
			rpg3D::EntityIds::defaultZoneEntrance/*instanceId*/,
			nullptr/*materialsList*/, StringUtil::EMPTY/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/,
			std::make_shared<std::string>(
				worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
				+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id
			)/*platformActivationScript*/
		);
	}

	return zoneContentData;
}

ZoneContentData::~ZoneContentData() {
	//void
}
