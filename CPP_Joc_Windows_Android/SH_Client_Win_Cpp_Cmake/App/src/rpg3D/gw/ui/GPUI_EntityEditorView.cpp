#include "GPUI_EntityEditorView.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/config/ToolConfigEntityEditor.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>
#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/config/entity/unit/T_Unit_EditorDefaultCreature_A_V1.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/IPayloadReceiverTargetModule.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <rpg3D/gw/manager/GPManager.h>
#include <rpg3D/gw/view/entityEditor/EntityEditorView.h>
#include <base/visual2D/VisualUtil2D.h>
#include <base/assets/IAssetsManager.h>
#include <base/app/IAppAssets.h>
#include <rpg3D/gw/entity/template/editableVoxelised/EditableVoxelisedTemplate.h>
#include <rpg3D/gw/entity/template/voxelizedEditor/VoxelizedEditorTemplate.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedModule.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorModule.h>
#include <base/exceptions/NotImplementedException.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

GPUI_EntityEditorView::GPUI_EntityEditorView(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_dispose_views->type, base::IGameWorld::GWEvent_saveData->type,
		std::bind(&GPUI_EntityEditorView::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	viewHideAnimationListener.cb_onAnimationFinished = std::bind(&GPUI_EntityEditorView::onViewHideAnimationFinished, this, std::placeholders::_1);
}

void GPUI_EntityEditorView::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGameWorld::GWEvent_dispose_views->type) {
		if (entityEditorView != nullptr) {
			gw->getViewComponentsHandler()->removeComponent(entityEditorView, true/*doDispose*/);

			entityEditorView = nullptr;
			gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onEntityEditor_deactivated);
		}
	} else if (event.type == base::IGameWorld::GWEvent_saveData->type) {
		if (isEditorActive) {
			stopCreatureCreator(false/*doAnimation*/);
		}
	}
	
	else {
		throw LogicException(LOC);
	}
}

bool GPUI_EntityEditorView::getIsEntityEditorActive() {
	if (entityEditorView) {
		return true;
	}
	
	return false;
}

void GPUI_EntityEditorView::startEntityEditor(int editorMode, bool isEditorStructureEnabled, Vector3* pos, IWorldEntity* existingEntityToEditOptional, IWorldEntity* existingEntityToEditOptionalB) {
	if (isEditorActive || entityEditorView != nullptr) {
		// Editor already active or hiding.
		return;
	}
	isEditorActive = true;

	GlobalAccessLogHandler::devLogHandler->post_debug("Enabling creature creator.");

	// Disable the in_game U.I.
	activateMainUIMinimalModeIncremental();

	// Set control mode to first person locked.
	prevValue_cameraMode = gw->getGPSim()->getCameraMode();
	prevValue_entityControlType = gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->getEntityControlType();

	// Don't change the camera any more. Auto camera mode changes feel bad.
	//gw->getGPSim()->setCameraMode(GameCameraModeBitGroups::followerFirstPersonTargetRotation, true);
	//gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes);
	
	// Do a player entity stop action.
	editorEntity = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/);
	float angZDeg = Math::atan2Deg(editorEntity->getY() - pos->y, editorEntity->getX() - pos->x);

	// Extract the player stack top entity if any and set that for editing if it is editable_creature.
	entityToEdit = existingEntityToEditOptional;
	if (entityToEdit != nullptr) {
		rpg3D::IStackableModule* entityToEdit_stackable = entityToEdit->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
		if (entityToEdit_stackable->getIsHauled()) {
			entityToEdit_stackable->getHauler()->addAction_StackableThrowActionContinuous(
				entityToEdit->getWrapper(), 
				nullptr, std::make_shared<Vector3>(pos),
				-1,
				std::make_shared<float>(angZDeg)/*throwRotZDeg*/
			);
		}
	}else {
		// Create a new enttity at the edit pos (platform pos) and make it look at the player.
		std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();
		extraData->setData(rpg3D::T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(rpg3D::T_Unit_EditorDefaultCreature_A_V1::flatPad_voxelGridZXY, -1));
		extraData->setData(rpg3D::T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(rpg3D::T_Unit_EditorDefaultCreature_A_V1::flatPad_materialsList, -1));

		Quaternion rot{ Vector3::Z, angZDeg };

		std::shared_ptr<std::string> instanceId = entityEditor_getNewEntityInstanceId();

		IWorldEntity* creatureEntity = gw->getGPMain()->createEntity(
			T_Unit_EditorDefaultCreature_A_V1::getNew(
				T_Unit_EditorDefaultCreature_A_V1::default_voxelSize, T_Unit_EditorDefaultCreature_A_V1::default_voxelGridOffset,
				T_Unit_EditorDefaultCreature_A_V1::default_fixedSizeAABBToushShape_min, T_Unit_EditorDefaultCreature_A_V1::default_fixedSizeAABBToushShape_max,
				T_Unit_EditorDefaultCreature_A_V1::default_stackable_haulOffsetZM
			),
			*pos,
			&rot/*rot*/, nullptr/*cardinalRotation*/,
			instanceId/*instanceId*/, -1/*ownerPlayerId*/,
			extraData,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[this](IWorldEntity* entity) -> void {
				rpg3D::ITeamModule* editorEntity_team = editorEntity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);

				rpg3D::ITeamModule* entity_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				entity_team->setTeamIndex(editorEntity_team->getTeamIndex());
			}
		);

		entityEditor_onEntityCreated(creatureEntity);

		entityToEdit = creatureEntity;
	}

	createNewView();

	// Initialise the edit modules on the editable and editor entities.
	rpg3D::entityEditorModule::IVoxelizedEditorModule* editorEntity_entityEditor = editorEntity->getComponentAs<rpg3D::entityEditorModule::IVoxelizedEditorModule*>(true/*mustExist*/);
	rpg3D::IEditableVoxelisedModule* entityToEdit_editableEntity = entityToEdit->getComponentAs<rpg3D::IEditableVoxelisedModule*>(true/*mustExist*/);

	editorEntity_entityEditor->startEdit(
		this, 
		entityToEdit_editableEntity, existingEntityToEditOptionalB, 
		entityEditorView,
		isEditorStructureEnabled,
		gw->getGPMain()->getCC()
	);

	// Create and activate the editor tool.
	addEditorTools(editorEntity, editorMode);

	entityEditorView->show(
		editorMode,
		true, nullptr, nullptr,
		gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>()->getBlockingInputLayer()
	);

	gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onEntityEditor_activated);
}

EntityEditorView* GPUI_EntityEditorView::createNewView() {
	GlobalAccessLogHandler::devLogHandler->post_debug("Creating a new creature creator.");

	if (entityEditorView != nullptr) {
		throw LogicException(LOC);
	}

	gw->getViewComponentsHandler()->addComponent(entityEditorView = new EntityEditorView(
		gw->getHandler()->getApp(), gw->getHandler()->getAppUtils()
	), true/*doCreate*/);
	gw->getGPMain()->getLayerSubUserInterface()->addChild(entityEditorView->getVisualsHolder());

	return entityEditorView;
}

void GPUI_EntityEditorView::addEditorTools(IWorldEntity* editorEntity, int editorMode) {
	rpg3D::IToolsHandlerModule* entity_toolsHandler = editorEntity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
	// These are the move to target, activate switches, buttons, levers, doors tools. Active selectable tools are not affected b this).
	entity_toolsHandler->setIsNonUserSelectableToolsEnabledIncremental(false);

	ArrayList< rpg3D::ToolConfigBase *> editorToolConfigsList{};
	if (editorMode == IEntityEditorView::EditorType::EDIT_OTHER) {
		editorToolConfigsList.appendDirect_chain(EntityTemplatesList::Tool_ToolEEPainter)
			->appendDirect_chain(EntityTemplatesList::Tool_ToolEEPlacer)
			->appendDirect_chain(EntityTemplatesList::Tool_ToolEERemoverSingle)
			->appendDirect_chain(EntityTemplatesList::Tool_ToolEERemoverLaser)
			->appendDirect_chain(EntityTemplatesList::Tool_ToolEEColorPicker);
	} else if (editorMode == IEntityEditorView::EditorType::PAINT_OTHER) {
		editorToolConfigsList.appendDirect_chain(EntityTemplatesList::Tool_ToolEEPainter)
			->appendDirect_chain(EntityTemplatesList::Tool_ToolEEColorPicker);
	} else {
		throw LogicException(LOC);
	}

	rpg3D::ToolConfigBase* defaultEditorToolConfig = EntityTemplatesList::Tool_ToolEEPainter;

	for (int i = 0; i < editorToolConfigsList.count(); i++) {
		rpg3D::ToolConfigBase* toolCfg = editorToolConfigsList.getDirect(i);

		sp<rpg3D::Tool> tool = toolCfg->createNewTool(
			gw->getAppAssets()
		);
		sp<rpg3D::ToolEntityEditor> cTool = tool.dCast<rpg3D::ToolEntityEditor>(true/*mustExtend*/);

		entity_toolsHandler->addTool(
			cTool
		);

		if (toolCfg == defaultEditorToolConfig) {
			entity_toolsHandler->selectTool(
				cTool,
				0
			);
		}
	}
}

void GPUI_EntityEditorView::removeEditorTools(IWorldEntity* editorEntity) {
	rpg3D::IToolsHandlerModule* entity_toolsHandler = editorEntity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

	std::shared_ptr<ArrayList<sp<Tool>>> toolsList = entity_toolsHandler->getAllToolWithTags(rpg3D::ToolConfigEntityEditor::TAG_entityEditor);
	if (toolsList != nullptr) {
		for (int i = 0; i < toolsList->count(); i++) {
			sp<Tool> tool = toolsList->getDirect(i);
			
			entity_toolsHandler->removeTool(tool);
		}
	}

	// Enable previously disabled auto_tools activation.
	entity_toolsHandler->setIsNonUserSelectableToolsEnabledIncremental(true);
}

void GPUI_EntityEditorView::entityEditor_requestStopEditor() {
	stopCreatureCreator(true/*doAnimation*/);
}

void GPUI_EntityEditorView::entityEditor_processOnEditOccured(IWorldEntity* entityToEdit) {
	entityEditor_onEditOccured(entityToEdit);
}

void GPUI_EntityEditorView::entityEditor_onEntityCreated(IWorldEntity* editEntity) {
	throw NotImplementedException();
}

std::shared_ptr<std::string> GPUI_EntityEditorView::entityEditor_getNewEntityInstanceId() {
	throw NotImplementedException();
}

void GPUI_EntityEditorView::entityEditor_onEditOccured(IWorldEntity* editEntity) {
	throw NotImplementedException();
}

void GPUI_EntityEditorView::entityEditor_onEditorClosed() {
	throw NotImplementedException();
}

void GPUI_EntityEditorView::stopCreatureCreator(bool doAnimation) {
	if (!isEditorActive) {
		return;
	}
	isEditorActive = false;

	GlobalAccessLogHandler::devLogHandler->post_debug("Disabling creature creator.");

	if (entityEditorView == nullptr) {
		throw LogicException(LOC);
	}

	rpg3D::entityEditorModule::IVoxelizedEditorModule* editorEntity_entityEditor = editorEntity->getComponentAs<rpg3D::entityEditorModule::IVoxelizedEditorModule*>(true/*mustExist*/);
	editorEntity_entityEditor->endEditIfAny();

	// The painter view is busy, add it to the destroying list.
	entityEditorView->hide(doAnimation, &viewHideAnimationListener);

	// Deactivate and remove the editor tool.
	removeEditorTools(editorEntity);
	
	// Do a template instant save.
	entityEditor_onEditorClosed();

	// Enable the in_game U.I.
	deactivateMainUIMinimalModeIncremental();

	// Set control mode to previously used.
	gw->getGPSim()->setCameraMode(prevValue_cameraMode, true);
	gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(prevValue_entityControlType);
}

void GPUI_EntityEditorView::onViewHideAnimationFinished(IGenericDrawable* target) {
	// This listener is used only for the panel hide.

	GlobalAccessLogHandler::devLogHandler->post_debug("Painter hide finished.");

	if (entityEditorView != target) {
		throw LogicException(LOC);
	}

	gw->getViewComponentsHandler()->removeComponent(entityEditorView, true/*doDispose*/);
	entityEditorView = nullptr;

	gw->dispatchEvent(*IGamePartUserInterface::GWEvent_onEntityEditor_deactivated);
}

void GPUI_EntityEditorView::disposePrePre() {
	if (entityEditorView != nullptr) {
		gw->getViewComponentsHandler()->removeComponent(entityEditorView, true/*doDispose*/);

		entityEditorView = nullptr;
	}

	super::disposePrePre();
}

GPUI_EntityEditorView::~GPUI_EntityEditorView() {
	//void
}
