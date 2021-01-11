#include "TZEA_Tool.h"
#include <rpg3D/gw/entity/module/zoneEditor/IZoneEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "../util/ToolActivationInfo_GroundPos.h"
#include "../util/ToolActivationInfo_None.h"
#include "../util/ToolActivationInfo_Entity.h"
#include <base/math/Math.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/flags/IFlags.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/config/entity/EntityIds.h>
#include <worldGame3D/gw/util/WrappedWorldFlag.h>
#include <base/math/util/BoolUtil.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/list/Array1D.h>
#include <base/log/GlobalAccessLogHandler.h>

#include <base/view/snackbar/util/WrappedSnackbarView.h>
#include <base/view/snackbar/ISnackbarView.h>
#include <base/view/snackbar/util/Config.h>
#include <base/app/IApp.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Doorframe_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Pillar_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_WallElement_A_V1.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace rpg3D;

const int TZEA_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

std::string TZEA_Tool::existingEntitiesEditModeIconRegionIds[3]{
		"rpg3D/gw/zoneEditor/tools/assets/multiControlA_moveEntities",
		"rpg3D/gw/zoneEditor/tools/assets/multiControlA_rotateEntities",
		"rpg3D/gw/zoneEditor/tools/assets/multiControlA_deleteEntities"
};

TZEA_Tool::TZEA_Tool(ToolConfigZEAssets* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::EntityDown)
		->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf, rpg3D::WorldOrEntityInteractionFilter::Flags::HasInstanceId))
		->addInstanceIdCheck(msp<Array1D<std::shared_ptr<std::string>>>(3, new std::shared_ptr<std::string>[3]{ EntityIds::groundTiledXY, EntityIds::underGroundTiledXY, EntityIds::wallsFullTileSizeTiledXY }), false/*mustOrMustNotEqual*/);
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::GroundDown);

	activationFiltersList_activeContinuousForUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::GroundMove);

	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main);
	activationFiltersList_activeContinuousForDeactivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main);

	//>>
	activationFiltersList_alwaysUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn);
	activationFiltersList_alwaysUpdate->append_emplace((int)IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn);
}

void TZEA_Tool::create(IAssetsManager* assetsManager) {
	super::create(assetsManager);

	secondaryControl_entityEditMode = addExtraActivationSecondaryControl("visual2D/taGeneral.atlas", existingEntitiesEditModeIconRegionIds[existingEntitiesEditMode]);

	listControl_asset_None = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/None", false/*isSelected*/);
	listControl_asset_Tree = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/tree", true/*isSelected*/);
	listControl_asset_Flower = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/flower", false/*isSelected*/);
	listControl_asset_pillar = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/pillar", false/*isSelected*/);
	listControl_asset_wallSegment = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/wallSegment", false/*isSelected*/);
	listControl_asset_doorframe = addExtraActivationListControl("visual2D/taGeneral.atlas", "rpg3D/gw/zoneEditor/tools/assets/doorframe", false/*isSelected*/);
}

bool TZEA_Tool::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::innactiveForActivation) {
		// This check is used because there are 2 separate activation filter that will be triggered on the same frame cursor_down.
		if (isToolActivationFilterReceived) {
			goto end;
		}
		isToolActivationFilterReceived = true;

		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::EntityDown) {
			sp<ToolActivationInfo_Base> touchData = new ToolActivationInfo_Entity(interaction->cursorIndex, interaction->targetWEntity);
			onFilterInteraction_innactiveForActivation(touchData);
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown) {
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
		} else if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn) {
			// This doesn't activate the tool, it just does a config change.
			if (interaction->toolInstanceId != instanceId || getIsOperatingAny()) {
				goto end;
			}

			toggleNextExistingEntitiesEditMode();
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

int TZEA_Tool::computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) {
	// Use this extra var to avoid commenting the other return. This is to avoid bugs when the touch mode is not guaranteed to bhe tool if it changes in the future.
	bool alwaysUseToolTouchMode = true | true;
	if (alwaysUseToolTouchMode) {
		// Always use tool touc mode. The zone editor doesn't have extra tool touch modes (eg. entity editor has editing entity rotation as a extra touch mode).
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(touchData);
}

void TZEA_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		isToolActivationFilterReceived = false;

		isPlacementOccured = false;
		isInitialGroundPosProcessed = false;
		forceMoveNewlyPlacedAsset = false;
		placedWEntity = nullptr;
	}
}

void TZEA_Tool::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		wWFlag_zoneEditorAssetInstanceIdCounter = nullptr;
	} else {
		wWFlag_zoneEditorAssetInstanceIdCounter = entity->getGW()->getFlags().get(EntityIds::gwFlagName_zoneEditorAssetInstanceIdCounter, true/*mustExist*/);
	}
}

void TZEA_Tool::tickActiveTouchMode(
	int touchMode, float deltaS,
	ToolActivationInfo_Base* touchData)
{
	super::tickActiveTouchMode(
		touchMode, deltaS,
		touchData
	);

	if (touchMode == TouchMode_TOOL) {
		if (touchData->getType() == ToolActivationInfo_GroundPos::TYPE) {
			ToolActivationInfo_GroundPos* cTouchData = touchData->castAs<ToolActivationInfo_GroundPos*>(ToolActivationInfo_GroundPos::TYPE);
			
			// Round the x,y pos components to 0.1m;
			Vector3 pos{
				Math::round(cTouchData->groundPos.x / 0.1f) * 0.1f,
				Math::round(cTouchData->groundPos.y / 0.1f) * 0.1f,
				cTouchData->groundPos.z
			};

			if (Math::dist(entity->getX(), entity->getY(), cTouchData->groundPos.x, cTouchData->groundPos.y) > EDIT_RANGE) {
				// The edit range is 5m from the player.
				return;
			}

			if (!isPlacementOccured) {
				isPlacementOccured = true;
				isInitialGroundPosProcessed = false;

				// Force move on newly placed assets.
				forceMoveNewlyPlacedAsset = true;

				EntityTemplate* entityTemplate = nullptr;
				if (getSelectedExtraActivationListControl() == nullptr || getSelectedExtraActivationListControl() == listControl_asset_None) {
					entityTemplate = nullptr;
				} else if (getSelectedExtraActivationListControl() == listControl_asset_Tree) {
					entityTemplate = rpg3D::EntityTemplatesList::Flora_TreeA;
				} else if (getSelectedExtraActivationListControl() == listControl_asset_Flower) {
					entityTemplate = rpg3D::EntityTemplatesList::Flora_SmallFlower_A_V1;
				} else if (getSelectedExtraActivationListControl() == listControl_asset_pillar) {
					entityTemplate = squareTactics_a_v1::T_Pillar_A_V1::TEMPLATE;
				} else if (getSelectedExtraActivationListControl() == listControl_asset_wallSegment) {
					entityTemplate = squareTactics_a_v1::T_WallElement_A_V1::TEMPLATE;
				} else if (getSelectedExtraActivationListControl() == listControl_asset_doorframe) {
					entityTemplate = squareTactics_a_v1::T_Doorframe_A_V1::TEMPLATE;
				}
				
				else {
					throw LogicException(LOC);
				}

				if (entityTemplate != nullptr) {
					int& zoneEditorAssetInstanceIdCounter = wWFlag_zoneEditorAssetInstanceIdCounter->worldFlag->wValue->getReferenceAs_int();
					std::shared_ptr<std::string> instanceId = std::make_shared<std::string>(std::string(EntityIds::asset_prefix) + std::to_string(zoneEditorAssetInstanceIdCounter));
					// This will update the flag entry in the gw also and that is saved when the zone data is saved;
					zoneEditorAssetInstanceIdCounter++;

					// Check that there is no entity with the instanceId.
					if (entity->getGW()->getGPMain()->isAnyEntityWithInstanceId(*instanceId)) {
						// Not allowed to create entities with the same instanceId.
						throw LogicException(LOC);
					}

					Quaternion rot{ Vector3::Z, 0.0f/*rotZ*/ };
					IWorldEntity* placedEntity = entity->getGW()->getGPMain()->createEntity(
						entityTemplate/*entityTemplate*/,
						pos,
						&rot/*rot*/, nullptr/*cardinalRotation*/,
						instanceId/*instanceId*/,
						0,
						nullptr,
						true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
						nullptr/*entityPostInitCb*/
					);
					placedWEntity = placedEntity->getWrapper();
				}
			} else {
				if (!isInitialGroundPosProcessed) {
					isInitialGroundPosProcessed = true;
				} else {
					if (placedWEntity != nullptr && placedWEntity->entity != nullptr) {
						if (forceMoveNewlyPlacedAsset || existingEntitiesEditMode == Move) {
							placedWEntity->entity->setPos(pos);
						} else if (existingEntitiesEditMode == Rotate) {
							// This will rotate at max delta if cursor moved to the right or left.
							float delta = (cTouchData->groundPos.x - lastTickGroundPos.x) - (cTouchData->groundPos.y - lastTickGroundPos.y);

							if (delta != 0.0f) {
								float angZDeg = placedWEntity->entity->getRot()->getAngleAround(Vector3::Z);
								angZDeg = angZDeg + delta * 50.0f;// Math::floor((angZDeg + deltaX * 100.0f) / 15.0f) * 15.0f;

								placedWEntity->entity->setRot(Vector3::Z, angZDeg);
							}
						}
					}
				}

				lastTickGroundPos.set(cTouchData->groundPos);
			}
		}else if (touchData->getType() == ToolActivationInfo_Entity::TYPE) {
			// Use this check in case multiple entities are touched
			/// and also the tool calls this function each frame with the last input (the ToolActivationInfo_Entity if unless a cursor move occurs);
			if (!isPlacementOccured) {
				isPlacementOccured = true;
				forceMoveNewlyPlacedAsset = false;

				ToolActivationInfo_Entity* cTouchData = touchData->castAs<ToolActivationInfo_Entity*>(ToolActivationInfo_Entity::TYPE);

				if (existingEntitiesEditMode == Delete) {
					if (cTouchData->wEntity->entity != nullptr) {
						if (cTouchData->wEntity->entity->getTemplate()->hasTag(rpg3D::EntityTags::ZONE_EDITOR_NOT_DELETABLE)) {
							// Entity has a tag that prevents the zone editor from deleting it.
							if (snackbarWView_assetNotDeletable == nullptr || snackbarWView_assetNotDeletable->view == nullptr) {
								snackbarWView_assetNotDeletable = entity->getGW()->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewSnackbarViewAutoDisposable();
								snackbarWView_assetNotDeletable->view->showInOrderManaged(
									true,
									std::make_shared<base::snackbar::Config>(
										"Asset is not deletable."/*text*/,
										4.0f/*autoCloseTimerS*/,
										nullptr/*dynamicButtonConfig*/
									)
								);
							}
						}else {
							cTouchData->wEntity->entity->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false/*isRequestFromServer*/);
						}
					}
				} else if (existingEntitiesEditMode == Move || existingEntitiesEditMode == Rotate) {
					placedWEntity = cTouchData->wEntity;
					isInitialGroundPosProcessed = false;
				} else {
					throw LogicException(LOC);
				}
			}
		} else {
			throw LogicException(LOC);
		}
	}
}

void TZEA_Tool::toggleNextExistingEntitiesEditMode() {
	if (existingEntitiesEditMode == Move) {
		existingEntitiesEditMode = Rotate;
	}else if (existingEntitiesEditMode == Rotate) {
		existingEntitiesEditMode = Delete;
	}else if (existingEntitiesEditMode == Delete) {
		existingEntitiesEditMode = Move;
	} else {
		throw LogicException(LOC);
	}

	setExtraActivationSecondaryControlIcon(
		secondaryControl_entityEditMode,
		"visual2D/taGeneral.atlas", existingEntitiesEditModeIconRegionIds[existingEntitiesEditMode]
	);
}

TZEA_Tool::~TZEA_Tool() {
	//void
}
