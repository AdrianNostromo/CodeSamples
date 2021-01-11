#pragma once

#include <base/gh.h>
#include "../../config/ToolConfigZoneEditor.h"

namespace rpg3D {
class ToolConfigZEGroundTiled : public ToolConfigZoneEditor {priv typedef ToolConfigZoneEditor super;pub dCtor(ToolConfigZEGroundTiled);
    pub static std::string TAG_zeGroundTiles;

	pub explicit ToolConfigZEGroundTiled(
		std::string id,
		int filtersPriority,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume);

	sp<Tool> createNewTool(IAppAssets* appAssets) override;

    pub ~ToolConfigZEGroundTiled() override;
};
};
