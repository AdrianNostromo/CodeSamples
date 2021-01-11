#include "ITool.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/event/ToolEvent.h>

using namespace rpg3D;

const int ITool::FilterSource::innactiveForActivation = 1;
const int ITool::FilterSource::activeContinuousForUpdate = 2;
const int ITool::FilterSource::activeContinuousForDeactivation = 3;
const int ITool::FilterSource::alwaysUpdate = 4;

int ITool::GetNew_GroupID() {
	static int GroupID_COUNTER = 0;

	int ret = GroupID_COUNTER;
	GroupID_COUNTER++;

	return ret;
}

const int ITool::GROUP_ID_NONE = -1;

int ITool::getGroupID() {
	return -1;
}

int ITool::GetNew_ID() {
	static int ID_COUNTER = 0;

	int ret = ID_COUNTER;
	ID_COUNTER++;

	return ret;
}

rpg3D::ToolEvent* ITool::ToolEvent_extraActivationListControls_onSelectedChanged = new rpg3D::ToolEvent("ToolEvent_extraActivationListControls_onSelectedChanged");
rpg3D::ToolEvent* ITool::ToolEvent_extraActivationSecondaryControl_onConfigChanged = new rpg3D::ToolEvent("ToolEvent_extraActivationSecondaryControl_onConfigChanged");

int ITool::ActionGroup_ToolOperate = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
int ITool::ActionGroup_ToolStopOperation = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int ITool::ActionGroup_ToolTurning = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

// voidData::(void*)ITool*;
base::EntityEvent* ITool::EVENT_tool_onIsOperatingChanged = new base::EntityEvent(false, "EVENT_tool_onIsOperatingChanged");

base::EntityEvent* ITool::EVENT_directionalTool_isOperating_disabled = new base::EntityEvent(false, "EVENT_directionalTool_isOperating_disabled");
base::EntityEvent* ITool::EVENT_directionalTool_isOperating_enabled = new base::EntityEvent(false, "EVENT_directionalTool_isOperating_enabled");

base::EntityEvent* ITool::EVENT_tool_triggerPointOfNoReturn_start = new base::EntityEvent(false, "EVENT_tool_triggerPointOfNoReturn_start");
base::EntityEvent* ITool::EVENT_tool_extendedActivationFinished = new base::EntityEvent(false, "EVENT_tool_extendedActivationFinished");
base::EntityEvent* ITool::EVENT_tool_extendedActivationCancelled = new base::EntityEvent(false, "EVENT_tool_extendedActivationCancelled");

int ITool::ActionGroup_MovementTurningIdle = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
int ITool::ActionGroup_MovementTurningAlways = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int ITool::ActionGroup_MouseTurningDirect = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
int ITool::ActionGroup_MouseTurningIndirect = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();


ITool::ExtraActivationListControl::ExtraActivationListControl(int uid, std::string& atlasId, std::string& regionId)
	: key(uid), uid(key), atlasId(atlasId), regionId(regionId)
{
	//void
}

dOpsDef(ITool::ExtraActivationListControl, uid);

ITool::ExtraActivationListControl::~ExtraActivationListControl() {
	//void
}

ITool::ExtraActivationSecondaryControl::ExtraActivationSecondaryControl(int uid, std::string& atlasId, std::string& regionID)
	: key(uid), uid(key), atlasId(atlasId), regionID(regionID)
{
	//void
}

dOpsDef(ITool::ExtraActivationSecondaryControl, uid);

ITool::ExtraActivationSecondaryControl::~ExtraActivationSecondaryControl() {
	//void
}
