#include "THM_SelectedTools.h"
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <base/object/WrappedValue.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

THM_SelectedTools::THM_SelectedTools(
	IWorldEntity* entity,
	ToolsHandlerTemplate* t)
	: super(
		entity,
		t
	),
	slotSelectedToolsList(t->toolSlotsList->count())
{
	//void
}

void THM_SelectedTools::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	selectedToolsList.enumerateDirect([gwGameParts](ListDL<sp<Tool>>::Entry* listEntry, sp<Tool>& tool, bool& stopEnumeration) ->void {
		tool->connectToGameParts(gwGameParts);
	});
}

void THM_SelectedTools::disconnectFromGamePartsPre() {
	for (int i = 0; i < toolsList.count(); i++) {
		sp<Tool> tool = toolsList.getDirect(i);

		// This always occurs to handle optimisations cleanup.
		tool->disconnectFromGamePartsPre();
	}

	super::disconnectFromGamePartsPre();
}

int THM_SelectedTools::getSelectedToolsCount() {
	return selectedToolsList.count();
}

ListDL<sp<Tool>>& THM_SelectedTools::getSelectedToolsList() {
	return selectedToolsList;
}

int THM_SelectedTools::getSelectedToolsCount_userSelectable() {
	return selectedToolsList_userSelectable.count();
}

ListDL<sp<Tool>>& THM_SelectedTools::getSelectedToolsList_userSelectable() {
	return selectedToolsList_userSelectable;
}

sp<Tool> THM_SelectedTools::getToolAtSlot(int toolSlotIndex) {
	sp<Tool> toolAtSlot = slotSelectedToolsList.getDirect(toolSlotIndex);

	return toolAtSlot;
}

void THM_SelectedTools::selectTool(sp<Tool> tool, int toolSlotIndex) {
	if (tool == nullptr) {
		// Use deselect tool to remove a tool from a slot_index.
		throw LogicException(LOC);
	}
	if (toolsList.indexOf(tool) < 0) {
		// Tool must be registered before activation;
		throw LogicException(LOC);
	}

	if (tool->getIsToolSelected() && ((toolSlotIndex >= 0 && tool->getSelectedToolSlot() == t->toolSlotsList->getDirect(toolSlotIndex)) || (toolSlotIndex < 0 && tool->getSelectedToolSlot() == nullptr))) {
		// Tool is already selected at the target slot.
		return;
	}

	if (tool->getIsToolSelected()) {
		// Tool alread selected at a different slot, deselect it.
		deselectTool(tool);
	}

	//GlobalAccessLogHandler::devLogHandler->post_debug("selectTool id: %s, slotIndex: %i", tool->config->id.c_str(), toolSlotIndex);

	sp<Tool> toolAtSlot = toolSlotIndex >= 0 ? slotSelectedToolsList.getDirect(toolSlotIndex) : nullptr;
	if (toolAtSlot != nullptr) {
		// Don't dispatch events because this tool will be at the same slot and to avoid a deselect event that will cause problems with u.i. animation.
		deselectTool(toolAtSlot, false/*dispatchSlotEvents*/);
	}

	ToolSlotConfig* toolSlot = nullptr;
	if (toolSlotIndex >= 0) {
		toolSlot = t->toolSlotsList->getDirect(toolSlotIndex);

		if (slotSelectedToolsList.getDirect(toolSlotIndex) != nullptr) {
			// There is already a tool at the slot.
			throw LogicException(LOC);
		}
		slotSelectedToolsList.setDirect(toolSlotIndex, tool);
	}
	tool->listEntry_selectedToolsList = selectedToolsList.appendDirect(tool);
	if (tool->config->isUserSelectable) {
		tool->listEntry_selectedToolsList_userSelectable = selectedToolsList_userSelectable.appendDirect(tool);
	}

	tool->setCb_isSelectedAndGWCapabilitiesExist_changed(std::bind(&THM_SelectedTools::onToolIsSelectedAndGWCapabilitiesExistChanged, this, std::placeholders::_1, std::placeholders::_2));
	tool->selectedStateChanged(true, toolSlotIndex, toolSlot, gwGamePartsOptional);

	e->dispatchEvent(*EVENT_toolsHandler_tool_selected->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));

	if (tool->config->isUserSelectable) {
		e->dispatchEvent(*EVENT_toolsHandler_userSelectableTool_selected->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
	}

	if (toolSlotIndex >= 0) {
		e->dispatchEvent(*EVENT_toolsHandler_slot_occupied->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
	}
}

void THM_SelectedTools::deselectTool(sp<Tool> tool, bool dispatchSlotEvents) {
	if (toolsList.indexOf(tool) < 0) {
		// Tool must be registered before activation;

		throw LogicException(LOC);
	}

	int toolSlotIndex = -1;
	if (tool->getSelectedToolSlot() != nullptr) {
		if (slotSelectedToolsList.getDirect(tool->getSelectedToolSlotIndex()) != tool) {
			throw LogicException(LOC);
		}

		toolSlotIndex = tool->getSelectedToolSlotIndex();

		slotSelectedToolsList.setDirect(tool->getSelectedToolSlotIndex(), nullptr);
	}
	
	//GlobalAccessLogHandler::devLogHandler->post_debug("deselectTool id: %s, slotIndex: %i", tool->config->id.c_str(), toolSlotIndex);

	if (tool->config->isUserSelectable) {
		tool->listEntry_selectedToolsList_userSelectable->remove();
		tool->listEntry_selectedToolsList_userSelectable = nullptr;
	}
	tool->listEntry_selectedToolsList->remove();
	tool->listEntry_selectedToolsList = nullptr;

	tool->selectedStateChanged(false, -1, nullptr/*toolSlot*/, nullptr/*gwGameParts*/);
	// Make sure to clear the cb after the above call so it is called while in the above function call.
	tool->setCb_isSelectedAndGWCapabilitiesExist_changed(nullptr);

	e->dispatchEvent(*EVENT_toolsHandler_tool_deselected->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
	
	if (tool->config->isUserSelectable) {
		e->dispatchEvent(*EVENT_toolsHandler_userSelectableTool_deselected->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
	}

	if (dispatchSlotEvents && toolSlotIndex >= 0) {
		e->dispatchEvent(*EVENT_toolsHandler_slot_unoccupied->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(tool, -1)));
	}
}

void THM_SelectedTools::onToolIsSelectedAndGWCapabilitiesExistChanged(ITool* target, bool isSelectedAndGWCapabilitiesExist) {
	if (!isSelectedAndGWCapabilitiesExist) {
		e->dispatchEvent(*EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(target->getSelfSP(), -1)));
	} else {
		e->dispatchEvent(*EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist->setWrappedValue(base::IWrappedValue::new_Custom<sp<ITool>>(target->getSelfSP(), -1)));
	}
}

void THM_SelectedTools::deselectAllToolsAtSlot(int toolSlotIndex) {
	sp<Tool> tool = slotSelectedToolsList.getDirect(toolSlotIndex);

	if (tool != nullptr) {
		deselectTool(tool);
	}
}

void THM_SelectedTools::selectSingleToolInGroup(int groupId, int toolId, int toolSlotIndex) {
	if (groupId < 0) {
		// Must use the groups and id system for this logic.
		throw LogicException(LOC);
	}

	// Enumerate all selected tools and deselect if the same groupId and different id.
	selectedToolsList.enumerateDirect([this, groupId, toolId](ListDL<sp<Tool>>::Entry* listEntry, sp<Tool>& tool, bool& stopEnumeration) ->void {
		if (tool->getGroupID() == groupId) {
			if (tool->getId() != toolId) {
				// Tool is always selected at this location.
				deselectTool(tool);
			}
		}
	});

	// Enumerate all tools and select the first one that has the groupId and toolId.
	// Error if multiple tools exist.
	bool isTargetToolFound = false;
	for(int i=0;i< toolsList.count();i++) {
		sp<Tool> tool = toolsList.getDirect(i);

		if (tool->getGroupID() == groupId) {
			if (tool->getId() == toolId) {
				if (isTargetToolFound) {
					// Tool with the same id is already active.
					throw LogicException(LOC);
				}
				isTargetToolFound = true;

				// Tool is always selected at this location.
				selectTool(tool, toolSlotIndex);
			}
		}
	};

	// Allow for toolId -1 to deselect all.
	// Error if tool not found.
	if (toolId >= 0 && !isTargetToolFound) {
		// Logic block not found but requested.
		throw LogicException(LOC);
	}
}

void THM_SelectedTools::onToolRemovePre(sp<Tool> tool) {
	super::onToolRemovePre(tool);
	
	if (tool->getIsToolSelected()) {
		deselectTool(tool);
	}
}

void THM_SelectedTools::disposePre() {
	while (selectedToolsList.count() > 0) {
		deselectTool(selectedToolsList.getLast()->data);
	}

	super::disposePre();
}

THM_SelectedTools::~THM_SelectedTools() {
	//void
}
