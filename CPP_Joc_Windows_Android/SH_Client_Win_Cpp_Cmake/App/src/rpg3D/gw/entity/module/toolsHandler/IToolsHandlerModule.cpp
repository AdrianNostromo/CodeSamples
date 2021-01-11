#include "IToolsHandlerModule.h"

using namespace rpg3D;

int IToolsHandlerModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IToolsHandlerModule);

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_toolAdded = new base::EntityEvent(false, "EVENT_toolsHandler_toolAdded");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_toolRemovePre = new base::EntityEvent(false, "EVENT_toolsHandler_toolRemovePre");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_toolRemoved = new base::EntityEvent(false, "EVENT_toolsHandler_toolRemoved");

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_tool_selected = new base::EntityEvent(false, "EVENT_toolsHandler_tool_selected");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_tool_deselected = new base::EntityEvent(false, "EVENT_toolsHandler_tool_deselected");

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist = new base::EntityEvent(false, "EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing = new base::EntityEvent(false, "EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing");

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_userSelectableTool_selected = new base::EntityEvent(false, "EVENT_toolsHandler_userSelectableTool_selected");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_userSelectableTool_deselected = new base::EntityEvent(false, "EVENT_toolsHandler_userSelectableTool_deselected");

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled = new base::EntityEvent(false, "EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled = new base::EntityEvent(false, "EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled");

base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_slot_occupied = new base::EntityEvent(false, "EVENT_toolsHandler_slot_occupied");
base::EntityEvent* IToolsHandlerModule::EVENT_toolsHandler_slot_unoccupied = new base::EntityEvent(false, "EVENT_toolsHandler_slot_unoccupied");
