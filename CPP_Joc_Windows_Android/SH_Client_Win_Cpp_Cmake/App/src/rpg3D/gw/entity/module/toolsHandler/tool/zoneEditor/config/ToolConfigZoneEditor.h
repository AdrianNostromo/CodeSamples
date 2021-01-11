#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigBase.h>

namespace rpg3D {
class ToolConfigZoneEditor : public ToolConfigBase {priv typedef ToolConfigBase super;pub dCtor(ToolConfigZoneEditor);
	pub static std::string TAG_zoneEditor;

	// The cooldown is mostly currently for spam limiting.
	// A good value for this is 0.001s (10ms). That blocks same frame activations but allows activations on individual frames;
	pub float cooldownS;

	pub std::string* activationSoundConfig_id;
	pub float activationSoundConfig_volume;

	pub explicit ToolConfigZoneEditor(
		std::string id, 
		std::shared_ptr<Array1D<std::string>> tagsList,
		int filtersPriority,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string& shortName_partA, std::string& shortName_partB,
		float cooldownS, float channelingDurationS,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume);

    pub ~ToolConfigZoneEditor() override;
};
};
