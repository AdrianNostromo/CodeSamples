#include "VEM_ColorsSelectorHandler.h"
#include <rpg3D/gw/view/entityEditor/IEntityEditorView_Edit.h>
#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/painter/config/ToolConfigEEPainter.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/placer/config/ToolConfigEEPlacer.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/colorPicker/config/ToolConfigEEColorPicker.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

VEM_ColorsSelectorHandler::VEM_ColorsSelectorHandler(
	IWorldEntity* entity,
	VoxelizedEditorTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

void VEM_ColorsSelectorHandler::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::ToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type || event.type == rpg3D::ToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type) {
		if (toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
			//Currently only 1 active user selectable tool may be displayed with this u.i.
			throw LogicException(LOC);
		}

		sp<Tool> selectedManualTool = nullptr;
		if (toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
			selectedManualTool = toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
		}
		if (selectedManualTool != nullptr && (selectedManualTool->hasTags(ToolConfigEEPainter::TAG_eePainter) || selectedManualTool->hasTags(ToolConfigEEPlacer::TAG_eePlacer) || selectedManualTool->hasTags(ToolConfigEEColorPicker::TAG_eeColorPicker))) {
			entityEditorView->setIsColorsSelectorActive(true);
		} else {
			entityEditorView->setIsColorsSelectorActive(false);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void VEM_ColorsSelectorHandler::onStartEditPost() {
	super::onStartEditPost();

	toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

	if (toolsHandler->getSelectedToolsCount_userSelectable() > 1) {
		//Currently only 1 active user selectable tool may be displayed with this u.i.
		throw LogicException(LOC);
	}

	sp<Tool> selectedManualTool = nullptr;
	if (toolsHandler->getSelectedToolsCount_userSelectable() == 1) {
		selectedManualTool = toolsHandler->getSelectedToolsList_userSelectable().getFirst()->data;
	}
	if (selectedManualTool != nullptr && (selectedManualTool->hasTags(ToolConfigEEPainter::TAG_eePainter) || selectedManualTool->hasTags(ToolConfigEEPlacer::TAG_eePlacer) || selectedManualTool->hasTags(ToolConfigEEColorPicker::TAG_eeColorPicker))) {
		entityEditorView->setIsColorsSelectorActive(true);
	} else {
		entityEditorView->setIsColorsSelectorActive(false);
	}
	
	// Hook the active tool change listenet.
	e->addEventListener(
		rpg3D::ToolsHandlerModule::EVENT_toolsHandler_slot_occupied->type, rpg3D::ToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied->type,
		std::bind(&VEM_ColorsSelectorHandler::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		local_autoListenersList
	);
}

void VEM_ColorsSelectorHandler::onEndEditPre() {
	super::onEndEditPre();

	// Clear the active tool change listener.
	local_autoListenersList.clear();
}

VEM_ColorsSelectorHandler::~VEM_ColorsSelectorHandler() {
	//void
}
