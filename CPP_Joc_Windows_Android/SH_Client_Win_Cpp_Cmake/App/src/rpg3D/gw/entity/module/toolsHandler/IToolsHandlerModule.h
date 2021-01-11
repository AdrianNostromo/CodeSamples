#pragma once

#include <base/gh.h>
#include <memory>
#include <base/sp.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>

class IAssetsManager;
namespace rpg3D {
	class Tool;
};

namespace rpg3D {
class IToolsHandlerModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_toolAdded;
	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_toolRemovePre;
	pub static base::EntityEvent* EVENT_toolsHandler_toolRemoved;

	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_tool_selected;
	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_tool_deselected;

	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_tool_selectedAndRequiredGWCapabilitiesExist;
	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_tool_deselectedOrRequiredGWCapabilitiesMissing;

	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_userSelectableTool_selected;
	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_userSelectableTool_deselected;
	
	pub static base::EntityEvent* EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled;
	pub static base::EntityEvent* EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled;

	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_slot_occupied;
	// wrappedValue = tool::sp<ITool>
	pub static base::EntityEvent* EVENT_toolsHandler_slot_unoccupied;

	pub virtual bool getIsAutomaticToolsEnabled() = 0;
	// These are the move to target, activate switches, buttons, levers, doors tools. Active selectable tools are not affected b this).
	pub virtual void setIsNonUserSelectableToolsEnabledIncremental(bool isEnabled) = 0;

	pub virtual ArrayList<sp<Tool>>* peekToolsList() = 0;
	pub virtual ArrayList<sp<Tool>>* peekToolsList_userSelectable() = 0;

	// toolCreateAssetsManager is used for tools that are constructed in the function call. It is used to have 1 line per tool creation for simple movement and turning tools).
	pub virtual void addTool(sp<Tool> tool, IAssetsManager* toolCreateAssetsManager = nullptr) = 0;
	pub virtual void removeTool(sp<Tool> tool) = 0;

	pub virtual int getSelectedToolsCount() = 0;
	pub virtual ListDL<sp<Tool>>& getSelectedToolsList() = 0;
	
	pub virtual int getSelectedToolsCount_userSelectable() = 0;
	pub virtual ListDL<sp<Tool>>& getSelectedToolsList_userSelectable() = 0;

	pub virtual bool hasTool(sp<Tool> tool) = 0;
	pub virtual bool hasTool_userSelectable(sp<Tool> tool) = 0;
	pub virtual sp<Tool> getToolAtSlot(int toolSlotIndex) = 0;

	// This can be used to deselect tools.
	pub virtual void selectTool(sp<Tool> tool, int toolSlotIndex) = 0;
	
	pub virtual void deselectTool(sp<Tool> tool, bool dispatchSlotEvents=true) = 0;
	pub virtual void deselectAllToolsAtSlot(int toolSlotIndex) = 0;
	
	// This can be used to deselect tools.
	pub virtual void selectSingleToolInGroup(int groupId, int toolId, int toolSlotIndex) = 0;

	pub virtual int getUserSelectableToolsCount() = 0;

	pub virtual bool getIsTurnBasedToolsModeActive() = 0;
	pub virtual void setIsTurnBasedToolsModeActive(bool isTurnBasedToolsModeActive) = 0;

	pub virtual int getTurnBasedActionStateCounter() = 0;
	pub virtual void incrementTurnBasedActionStateCounter() = 0;

	pub virtual sp<Tool> getFirstToolWithTags(ArrayList<std::string>& tagsList) = 0;
	pub virtual sp<Tool> getFirstToolWithTags(Array1D<std::string>& tagsList) = 0;
	pub virtual sp<Tool> getFirstToolWithTags(std::string& singleTag) = 0;

	pub virtual std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(ArrayList<std::string>& tagsList) = 0;
	pub virtual std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(Array1D<std::string>& tagsList) = 0;
	pub virtual std::shared_ptr<ArrayList<sp<Tool>>> getAllToolWithTags(std::string& singleTag) = 0;

	pub virtual bool hasToolWithTags(ArrayList<std::string>& tagsList) = 0;
	pub virtual bool hasToolWithTags(Array1D<std::string>& tagsList) = 0;
	pub virtual bool hasToolWithTags(std::string singleTag) = 0;

	pub template <typename T>
	T getToolWithTagsCastAs(ArrayList<std::string>& tagsList);
	pub template <typename T>
	T getToolWithTagsCastAs(Array1D<std::string>& tagsList);
	pub template <typename T>
	T getToolWithTagsCastAs(std::string& singleTag);

	pub ~IToolsHandlerModule() override = default;
};

template <typename T>
T IToolsHandlerModule::getToolWithTagsCastAs(ArrayList<std::string>& tagsList) {
	sp<Tool> tool = getFirstToolWithTags(tagsList);
	if (tool == nullptr) {
		return nullptr;
	}

	T cTool = tool->castAs<T>();
	if (cTool == nullptr) {
		throw LogicException(LOC);
	}

	return cTool;
}

template <typename T>
T IToolsHandlerModule::getToolWithTagsCastAs(Array1D<std::string>& tagsList) {
	sp<Tool> tool = getFirstToolWithTags(tagsList);
	if (tool == nullptr) {
		return nullptr;
	}

	T cTool = tool->castAs<T>();
	if (cTool == nullptr) {
		throw LogicException(LOC);
	}

	return cTool;
}

template <typename T>
T IToolsHandlerModule::getToolWithTagsCastAs(std::string& singleTag) {
	sp<Tool> tool = getFirstToolWithTags(singleTag);
	if (tool == nullptr) {
		return nullptr;
	}

	T cTool = tool->castAs<T>();
	if (cTool == nullptr) {
		throw LogicException(LOC);
	}

	return cTool;
}

};
