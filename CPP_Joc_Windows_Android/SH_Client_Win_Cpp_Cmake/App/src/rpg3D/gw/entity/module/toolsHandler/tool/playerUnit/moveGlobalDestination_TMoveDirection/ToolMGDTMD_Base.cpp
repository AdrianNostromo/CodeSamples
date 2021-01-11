#include "ToolMGDTMD_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMGDTMD_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveGlobalDestination_TMoveDirection", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MGDTMD"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMGDTMD_Base::ToolMGDTMD_Base(int movementTypeOverride, bool usesGridMovement, DestinationType destinationType)
	: super(ToolConfig),
	movementTypeOverride(movementTypeOverride), usesGridMovement(usesGridMovement), destinationType(destinationType)
{
	//void
}

bool ToolMGDTMD_Base::canTrigger() {
	return true;
}

ToolMGDTMD_Base::~ToolMGDTMD_Base() {
	//void
}
