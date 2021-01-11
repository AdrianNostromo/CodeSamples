#include "TZEGT_Tool.h"
#include <rpg3D/gw/entity/module/zoneEditor/IZoneEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "../util/ToolActivationInfo_GroundPos.h"
#include "../util/ToolActivationInfo_None.h"
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/flags/IFlags.h>
#include <rpg3D/gw/config/entity/EntityIds.h>
#include <worldGame3D/gw/util/WrappedWorldFlag.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

const int TZEGT_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

TZEGT_Tool::TZEGT_Tool(ToolConfigZEGroundTiled* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::GroundDown);

	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::GroundMove);

	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main);
	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main);
	
	//>>
	activationFiltersList_alwaysUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn);
}

void TZEGT_Tool::create(IAssetsManager* assetsManager) {
	super::create(assetsManager);
	
	listControl_ground_None = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/groundTiled/None", false/*isSelected*/);
	listControl_ground_AB = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/groundTiled/AB", true/*isSelected*/);
	listControl_ground_A = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/groundTiled/A", false/*isSelected*/);
	listControl_ground_B = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/groundTiled/B", false/*isSelected*/);
}

bool TZEGT_Tool::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::innactiveForActivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_GroundPos(interaction->cursorIndex, interaction->worldPos);
			onFilterInteraction_innactiveForActivation(touchData);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GroundMove) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_GroundPos(interaction->cursorIndex, interaction->worldPos);
			onFilterInteraction_activeContinuousForUpdate(touchData);
		} else {
			localIsProcessed = false;
		}
	} else if (filterSource == FilterSource::activeContinuousForDeactivation) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_None(interaction->cursorIndex);
			onFilterInteraction_activeContinuousForDeactivation(touchData);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_None(interaction->cursorIndex);
			onFilterInteraction_activeContinuousForDeactivation(touchData);
		} else {
			localIsProcessed = false;
		}
	}
	
	else if (filterSource == FilterSource::alwaysUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn) {
			if (interaction->toolInstanceId != instanceId) {
				goto end;
			}
			
			TreeAVL<ExtraActivationListControl>::Node* extraActivationControlNode = extraActivationListControlsList->getNodeFromKey(interaction->toolExtraControlSourceId, false/*mustExist*/);
			if (extraActivationControlNode == nullptr || extraActivationControlNode->data.isSelected) {
				goto end;
			}

			// Item needs selecting.
			setExtraActivationListControlIsSelected(extraActivationControlNode, true/*isSelected*/);
		} else {
			localIsProcessed = false;
		}
	}

	else {
	localIsProcessed = false;
	}
end:

if (isProcessed && localIsProcessed) {
	// Filter used in multiple layers. That should never occur. Filters code is not made for that.
	throw LogicException(LOC);
}

return isProcessed || localIsProcessed;
}

int TZEGT_Tool::computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) {
	// Use this extra var to avoid commenting the other return. This is to avoid bugs when the touch mode is not guaranteed to bhe tool if it changes in the future.
	bool alwaysUseToolTouchMode = true | true;
	if (alwaysUseToolTouchMode) {
		// Always use tool touc mode. The zone editor doesn't have extra tool touch modes (eg. entity editor has editing entity rotation as a extra touch mode).
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(touchData);
}

void TZEGT_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		// This makes sure to not ignore the first touch.
		dontProcessSameGPos = false;
	}
}

void TZEGT_Tool::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		wWFlag_zoneEditorTileCellSizeCm = nullptr;

		entityToEdit = nullptr;
		entityToEdit_visual3D = nullptr;
		entityToEdit_visual3D_editingVoxelsVisual = nullptr;
		entityToEdit_physics3D = nullptr;

		entityToEditB = nullptr;
		entityToEditB_visual3D = nullptr;
		entityToEditB_visual3D_editingVoxelsVisual = nullptr;
	} else {
		wWFlag_zoneEditorTileCellSizeCm = entity->getGW()->getFlags().get(EntityIds::gwFlagName_zoneEditorTileCellSizeCm, true/*mustExist*/);

		entityToEdit = entity->getGW()->getGPMain()->getEntityWithInstanceId(*EntityIds::groundTiledXY, true/*mustExist*/);
		entityToEdit_visual3D = entityToEdit->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
		entityToEdit_visual3D_editingVoxelsVisual = entityToEdit_visual3D->getDrawableMustExistAs<VoxelGridDrawable3D*>("_VoxelGridGroup_");
		entityToEdit_physics3D = entityToEdit->getComponentAs<rpg3D::IPhysics3DModule*>(true/*mustExist*/);

		entityToEditB = entity->getGW()->getGPMain()->getEntityWithInstanceId(*EntityIds::underGroundTiledXY, false/*mustExist*/);
		if (entityToEditB != nullptr) {
			entityToEditB_visual3D = entityToEditB->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
			entityToEditB_visual3D_editingVoxelsVisual = entityToEditB_visual3D->getDrawableMustExistAs<VoxelGridDrawable3D*>("_VoxelGridGroup_");
		}
	}
}

void TZEGT_Tool::tickActiveTouchMode(
	int touchMode, float deltaS,
	ToolActivationInfo_Base* touchData)
{
	super::tickActiveTouchMode(
		touchMode, deltaS,
		touchData
	);

	if (touchMode == TouchMode_TOOL) {
		ToolActivationInfo_GroundPos* cTouchData = touchData->castAs<ToolActivationInfo_GroundPos*>(ToolActivationInfo_GroundPos::TYPE);

		if (Math::dist(entity->getX(), entity->getY(), cTouchData->groundPos.x, cTouchData->groundPos.y) > EDIT_RANGE) {
			// The edit range is 5m from the player.
			return;
		}

		Vector3* voxelsGridOffset = entityToEdit_visual3D_editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

		float tileSizeM = wWFlag_zoneEditorTileCellSizeCm->worldFlag->wValue->getDirectAs_int() / 100.0f;
		Vector2Int globalGPos{
			static_cast<int>(Math::floor(cTouchData->groundPos.x / tileSizeM)),
			static_cast<int>(Math::floor(cTouchData->groundPos.y / tileSizeM))
		};
		Vector2Int voxelsGridGPos{
			static_cast<int>(Math::floor((cTouchData->groundPos.x - (voxelsGridOffset->x - 0.5f) * tileSizeM) / tileSizeM)),
			static_cast<int>(Math::floor((cTouchData->groundPos.y - (voxelsGridOffset->y - 0.5f) * tileSizeM) / tileSizeM))
		};

		if (dontProcessSameGPos && voxelsGridGPos.equals(lastProcessedGPos)) {
			// Ignore same gPos touches. But don't ignore on the first touch input.
			return;
		}

		// Ignore gPos touches after the first gPos.
		dontProcessSameGPos = true;
		lastProcessedGPos.set(voxelsGridGPos);

		{
			Array3D<signed char>* tileVoxels3D = nullptr;
			if (getSelectedExtraActivationListControl() == nullptr || getSelectedExtraActivationListControl() == listControl_ground_None) {
				tileVoxels3D = rpg3D::TC_GroundOrWall::getOrCreateGrid_None_None();
			} else if (getSelectedExtraActivationListControl() == listControl_ground_AB) {
				tileVoxels3D = (((globalGPos.x + globalGPos.y) % 2) == 0) ? rpg3D::TC_GroundOrWall::getOrCreateGridGrassADirtA() : rpg3D::TC_GroundOrWall::getOrCreateGridGrassBDirtA();
			} else if (getSelectedExtraActivationListControl() == listControl_ground_A) {
				tileVoxels3D = rpg3D::TC_GroundOrWall::getOrCreateGridGrassADirtA();
			} else if (getSelectedExtraActivationListControl() == listControl_ground_B) {
				tileVoxels3D = rpg3D::TC_GroundOrWall::getOrCreateGridGrassBDirtA();
			} else {
				throw LogicException(LOC);
			}
			if (tileVoxels3D == nullptr) {
				throw LogicException(LOC);
			}

			Vector3Int destGPos{ voxelsGridGPos.x, voxelsGridGPos.y, 0 };
			Vector3Int postChangeGridBoundsMin{};
			entityToEdit_visual3D_editingVoxelsVisual->getVoxelsGrid()->setVoxelsRegion(
				destGPos, *tileVoxels3D, &postChangeGridBoundsMin/*postChangeGridBoundsMin*/
			);
			destGPos.x -= postChangeGridBoundsMin.x;
			destGPos.y -= postChangeGridBoundsMin.y;
		}

		if (entityToEditB_visual3D_editingVoxelsVisual != nullptr) {
			Array3D<signed char>* tileVoxels3D = nullptr;
			if (getSelectedExtraActivationListControl() == nullptr || getSelectedExtraActivationListControl() == listControl_ground_None) {
				tileVoxels3D = rpg3D::TC_GroundOrWall::getOrCreateGrid_None_None();
			} else if (getSelectedExtraActivationListControl() == listControl_ground_AB || getSelectedExtraActivationListControl() == listControl_ground_A || getSelectedExtraActivationListControl() == listControl_ground_B) {
				tileVoxels3D = rpg3D::TC_GroundOrWall::getOrCreateGridStoneA();
			} else {
				throw LogicException(LOC);
			}
			if (tileVoxels3D == nullptr) {
				throw LogicException(LOC);
			}

			Vector3Int destGPos{ voxelsGridGPos.x, voxelsGridGPos.y, 0 };
			Vector3Int postChangeGridBoundsMin{};
			entityToEditB_visual3D_editingVoxelsVisual->getVoxelsGrid()->setVoxelsRegion(
				destGPos, *tileVoxels3D, &postChangeGridBoundsMin/*postChangeGridBoundsMin*/
			);
			destGPos.x -= postChangeGridBoundsMin.x;
			destGPos.y -= postChangeGridBoundsMin.y;
		}

		{
			Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = entityToEdit_visual3D_editingVoxelsVisual->getVoxelsGrid()->peekGrid();
			int gCountX = voxelsGrid->getLengthA();
			int gCountY = voxelsGrid->getLengthB();

			std::shared_ptr<Array2D<signed char>> wallsTilingGrid = std::make_shared<Array2D<signed char>>(
				gCountX/*_lengthA*/, gCountY/*_lengthB*/
			);
			for (int gX = 0; gX < gCountX; gX++) {
				for (int gY = 0; gY < gCountY; gY++) {
					// No need to clip the range because only ==-1 and !=-1 is tested when physics mesh is generated.
					wallsTilingGrid->setDirect(gX, gY, voxelsGrid->getReference(gX, gY, 0).matIndex);
				}
			}

			// Walls tiled grid2D single entity
			std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
			tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneC());
			tilesConfigList->append_emplace(rpg3D::TC_GroundOrWall::getOrCreateGridStoneD());

			// This offset contains a default 0.5f, 0.5f offset because it uses centered_on_origin_voxels but the physics uses lower_left_centering, so do a -0.5f offset on x and y;
			Vector3* voxelsGridOffset = entityToEdit_visual3D_editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

			sp<Vector3> gridGeometryOffset = new Vector3{
				voxelsGridOffset->x,
				voxelsGridOffset->y,
				0.0f
			};
			std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
				rpg3D::TC_GroundOrWall::TileVoxelsCount_1Layer,
				VoxelSides::BitGroup_xMin | VoxelSides::BitGroup_xMax | VoxelSides::BitGroup_yMin | VoxelSides::BitGroup_yMax/* | VoxelSides::BitGroup_zMin*/ | VoxelSides::BitGroup_zMax/*enabledSidesTriangulationBitMask*/,
				tilesConfigList,
				wallsTilingGrid,
				gridGeometryOffset
			);

			entityToEdit_physics3D->setTilingInformation(tilingInformation);
		}

		GlobalAccessLogHandler::devLogHandler->post_debug("ground_tile %i %i", voxelsGridGPos.x, voxelsGridGPos.y);
	}
}

TZEGT_Tool::~TZEGT_Tool() {
	//void
}
