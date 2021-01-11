#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/config/ToolConfigEntityEditor.h>

namespace rpg3D {
class ToolConfigEEPainter : public ToolConfigEntityEditor {priv typedef ToolConfigEntityEditor super;pub dCtor(ToolConfigEEPainter);
    pub static std::string TAG_eePainter;

	pub explicit ToolConfigEEPainter(
		std::string id,
		int filtersPriority,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume);

	sp<Tool> createNewTool(IAppAssets* appAssets) override;

    pub ~ToolConfigEEPainter() override;
};
};
