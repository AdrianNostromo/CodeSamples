#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/config/ToolConfigEntityEditor.h>

namespace rpg3D {
class ToolConfigEERemoverSingle : public ToolConfigEntityEditor {priv typedef ToolConfigEntityEditor super;pub dCtor(ToolConfigEERemoverSingle);
    pub static std::string TAG_eeRemoverSingle;

	pub explicit ToolConfigEERemoverSingle(
		std::string id,
		int filtersPriority,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume);

	sp<Tool> createNewTool(IAppAssets* appAssets) override;

    pub ~ToolConfigEERemoverSingle() override;
};
};
