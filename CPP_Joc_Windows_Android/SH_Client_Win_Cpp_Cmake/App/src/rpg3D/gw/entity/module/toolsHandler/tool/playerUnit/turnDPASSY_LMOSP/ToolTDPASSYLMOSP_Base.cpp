#include "ToolTDPASSYLMOSP_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolTDPASSYLMOSP_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_turnDPASSY_LMOSP", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"TDPASSYLMOSP"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolTDPASSYLMOSP_Base::ToolTDPASSYLMOSP_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolTDPASSYLMOSP_Base::canTrigger() {
	return true;
}

ToolTDPASSYLMOSP_Base::~ToolTDPASSYLMOSP_Base() {
	//void
}
