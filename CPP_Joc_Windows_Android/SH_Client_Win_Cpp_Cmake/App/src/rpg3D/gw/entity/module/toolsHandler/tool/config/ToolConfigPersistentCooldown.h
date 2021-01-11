#pragma once

#include <base/gh.h>
#include "ToolConfigBase.h"

namespace rpg3D {
class ToolConfigPersistentCooldown : public ToolConfigBase {priv typedef ToolConfigBase super;pub dCtor(ToolConfigPersistentCooldown);
	// The cooldown is mostly currently for spam limiting.
	float cooldownS;

	pub explicit ToolConfigPersistentCooldown(
		std::string id, std::shared_ptr<Array1D<std::string>> tagsList,
		int filtersPriority, bool isUserSelectable, bool isDirectional, bool supportsContinuousReactivation, float channelingDurationS,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS);

    pub ~ToolConfigPersistentCooldown() override;
};
};
