#include "ToolMLS_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMLS_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveLocalStrafe", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MLS"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMLS_Base::ToolMLS_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolMLS_Base::canTrigger() {
	return true;
}

ToolMLS_Base::~ToolMLS_Base() {
	//void
}
