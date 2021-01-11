#include "ToolMGD_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMGD_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveGlobalDirection", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MGD"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMGD_Base::ToolMGD_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolMGD_Base::canTrigger() {
	return true;
}

ToolMGD_Base::~ToolMGD_Base() {
	//void
}
