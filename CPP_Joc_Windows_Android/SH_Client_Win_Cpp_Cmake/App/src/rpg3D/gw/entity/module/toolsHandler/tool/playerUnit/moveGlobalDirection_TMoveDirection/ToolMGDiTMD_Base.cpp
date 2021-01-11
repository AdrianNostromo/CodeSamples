#include "ToolMGDiTMD_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMGDiTMD_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveGlobalDirection_TMoveDirection", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MGDiTMD"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMGDiTMD_Base::ToolMGDiTMD_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolMGDiTMD_Base::canTrigger() {
	return true;
}

ToolMGDiTMD_Base::~ToolMGDiTMD_Base() {
	//void
}
