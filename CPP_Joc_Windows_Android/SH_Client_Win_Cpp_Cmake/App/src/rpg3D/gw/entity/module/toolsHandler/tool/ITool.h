#pragma once

#include <base/gh.h>
#include <memory>
#include <base/list/ArrayList.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/util/ManagedFilersList.h>
#include <base/list/ListDL.h>
#include <base/container/TreeAVL.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

namespace base {
	class EntityEvent;
};

namespace rpg3D {
	class IUserControlledModule;
	class IWorldInteractionFilter;
	class WorldOrEntityInteractionEvent;
	class ToolConfigBase;
	class ToolEvent;
}

namespace rpg3D {
class ITool {
	pub class FilterSource {
		pub static const int innactiveForActivation;
		pub static const int activeContinuousForUpdate;
		pub static const int activeContinuousForDeactivation;
		pub static const int alwaysUpdate;
	};

	pub class ExtraActivationListControl {pub dCtor(ExtraActivationListControl);
		// The var name is key because this param name is used in the tree to find entries.
		pub int key;
		pub int& uid;

		pub std::string atlasId;
		pub std::string regionId;

		pub bool isSelected = false;

		pub explicit ExtraActivationListControl(int uid, std::string& atlasId, std::string& regionId);
		
		pub dOpsDec(ExtraActivationListControl);

		pub ~ExtraActivationListControl();
	};
	pub class ExtraActivationSecondaryControl {pub dCtor(ExtraActivationSecondaryControl);
		// The var name is key because this param name is used in the tree to find entries.
		pub int key;
		pub int& uid;

		pub std::string atlasId;
		pub std::string regionID;

		pub explicit ExtraActivationSecondaryControl(int uid, std::string& atlasId, std::string& regionID);

		pub dOpsDec(ExtraActivationSecondaryControl);

		pub ~ExtraActivationSecondaryControl();
	};

	prot static int GetNew_GroupID();
	pub static const int GROUP_ID_NONE;
	pub virtual int getGroupID();

	prot static int GetNew_ID();
	// This is used for error detection when calling entity->getComponentAs(...);
	// This must be extended by interfaces that implement the ID;
	pub virtual int getId() = 0;

	pub virtual sp<ITool> getSelfSP() = 0;

	pub static rpg3D::ToolEvent* ToolEvent_extraActivationListControls_onSelectedChanged;
	pub static rpg3D::ToolEvent* ToolEvent_extraActivationSecondaryControl_onConfigChanged;

	pub virtual base::TypedEventDispatcher1D<rpg3D::ToolEvent>& getEvents() = 0;

	pub static int ActionGroup_ToolOperate;
	pub static int ActionGroup_ToolStopOperation;

	pub static int ActionGroup_ToolTurning;

	pub static base::EntityEvent* EVENT_tool_onIsOperatingChanged;

	pub static base::EntityEvent* EVENT_directionalTool_isOperating_disabled;
	pub static base::EntityEvent* EVENT_directionalTool_isOperating_enabled;

	pub static base::EntityEvent* EVENT_tool_triggerPointOfNoReturn_start;
	pub static base::EntityEvent* EVENT_tool_extendedActivationFinished;
	pub static base::EntityEvent* EVENT_tool_extendedActivationCancelled;

	pub static int ActionGroup_MovementTurningIdle;
	pub static int ActionGroup_MovementTurningAlways;

	pub static int ActionGroup_MouseTurningDirect;
	pub static int ActionGroup_MouseTurningIndirect;

	pub virtual int getInstanceId() = 0;
	pub virtual ToolConfigBase* getConfig() = 0;
	
	pub virtual int getFiltersPriority() = 0;

	pub virtual std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_innactiveForActivation() = 0;
	pub virtual std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_activeContinuousForUpdate() = 0;
	pub virtual std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_activeContinuousForDeactivation() = 0;
	pub virtual std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> getActivationFiltersList_alwaysUpdate() = 0;

	pub virtual rpg3D::ManagedFilersList* getCustomFiltersList_innactiveForActivation() = 0;
	pub virtual rpg3D::ManagedFilersList* getCustomFiltersList_activeContinuousForUpdate() = 0;
	pub virtual rpg3D::ManagedFilersList* getCustomFiltersList_activeContinuousForDeactivation() = 0;
	pub virtual rpg3D::ManagedFilersList* getCustomFiltersList_alwaysUpdate() = 0;

	pub virtual bool getIsOperatingAny() = 0;

	pub virtual bool onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) = 0;

	pub virtual TreeAVL<ExtraActivationListControl>* getExtraActivationListControlsList() = 0;
	pub virtual TreeAVL<ExtraActivationSecondaryControl>* getExtraActivationSecondaryControlsList() = 0;

	pub virtual ~ITool() = default;
};
};
