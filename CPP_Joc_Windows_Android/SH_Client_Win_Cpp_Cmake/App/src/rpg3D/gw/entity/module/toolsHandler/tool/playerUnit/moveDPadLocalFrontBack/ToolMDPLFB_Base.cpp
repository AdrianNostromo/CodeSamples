#include "ToolMDPLFB_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMDPLFB_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveDPadLocalFrontBack", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MDPLFB"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMDPLFB_Base::ToolMDPLFB_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolMDPLFB_Base::canTrigger() {
	return true;
}

ToolMDPLFB_Base::~ToolMDPLFB_Base() {
	//void
}
