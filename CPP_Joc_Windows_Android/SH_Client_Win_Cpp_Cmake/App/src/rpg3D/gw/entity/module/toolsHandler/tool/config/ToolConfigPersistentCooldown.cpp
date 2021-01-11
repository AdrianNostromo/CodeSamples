#include "ToolConfigPersistentCooldown.h"

using namespace rpg3D;

ToolConfigPersistentCooldown::ToolConfigPersistentCooldown(
	std::string id, std::shared_ptr<Array1D<std::string>> tagsList,
	int filtersPriority, bool isUserSelectable, bool isDirectional, bool supportsContinuousReactivation, float channelingDurationS,
	std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
	std::string shortName_partA, std::string shortName_partB,
	float cooldownS)
	: super(
		id, tagsList,
		filtersPriority, isUserSelectable, isDirectional, supportsContinuousReactivation, channelingDurationS,
		icon_atlasId, icon_regionId,
		shortName_partA, shortName_partB),
	cooldownS(cooldownS)
{
	//void
}

ToolConfigPersistentCooldown::~ToolConfigPersistentCooldown() {
	//void
}
