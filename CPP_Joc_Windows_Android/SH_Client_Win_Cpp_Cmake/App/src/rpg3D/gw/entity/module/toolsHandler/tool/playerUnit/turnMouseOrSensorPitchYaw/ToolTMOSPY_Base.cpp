#include "ToolTMOSPY_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolTMOSPY_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_turnMouseOrSensorPitchYaw", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"TMOSPY"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolTMOSPY_Base::ToolTMOSPY_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolTMOSPY_Base::canTrigger() {
	return true;
}

ToolTMOSPY_Base::~ToolTMOSPY_Base() {
	//void
}
