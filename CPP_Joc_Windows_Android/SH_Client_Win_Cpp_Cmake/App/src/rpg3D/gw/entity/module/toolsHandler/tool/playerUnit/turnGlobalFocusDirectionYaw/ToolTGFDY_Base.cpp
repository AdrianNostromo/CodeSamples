#include "ToolTGFDY_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolTGFDY_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_turnGlobalFocusDirectionYaw", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"TGFDY"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolTGFDY_Base::ToolTGFDY_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolTGFDY_Base::canTrigger() {
	return true;
}

ToolTGFDY_Base::~ToolTGFDY_Base() {
	//void
}
