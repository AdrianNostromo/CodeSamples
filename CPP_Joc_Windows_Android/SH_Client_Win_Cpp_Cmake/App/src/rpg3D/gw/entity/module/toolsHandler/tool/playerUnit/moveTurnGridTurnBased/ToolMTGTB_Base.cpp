#include "ToolMTGTB_Base.h"
#include <base/statics/StaticsInit.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolConfigBase* ToolMTGTB_Base::ToolConfig = base::StaticsInit::AddCbGeneral<ToolConfigBase*>(0, []() {
	ToolConfig = new ToolConfigBase(
		"Tool_moveTurnGridTurnBased", nullptr/*tagsList*/,
		rpg3D::ToolConfigBase::FiltersPriority::automaticMovement, false/*isUserSelectable*/, false/*isDirectional*/, false/*supportsContinuousReactivation*/, -1.0f/*channelingDurationS*/,
		nullptr/*icon_atlasId*/, nullptr/*icon_regionId*/,
		"MTGTB"/*shortName_partA*/, ""/*shortName_partB*/);
});

ToolMTGTB_Base::ToolMTGTB_Base()
	: super(ToolConfig)
{
	//void
}

bool ToolMTGTB_Base::canTrigger() {
	return true;
}

ToolMTGTB_Base::~ToolMTGTB_Base() {
	//void
}
