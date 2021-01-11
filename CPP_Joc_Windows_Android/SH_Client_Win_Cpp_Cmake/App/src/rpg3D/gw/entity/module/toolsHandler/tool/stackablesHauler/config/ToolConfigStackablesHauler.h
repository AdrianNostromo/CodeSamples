#pragma once

#include <base/gh.h>
#include <base/visual3d/util/Visual3DCreateConfig.h>
#include <base/audio/sound/util/CfgSound.h>
#include "../../config/ToolConfigBase.h"

namespace rpg3D {
class ToolConfigStackablesHauler : public ToolConfigBase {
private: typedef ToolConfigBase super;
	static std::string TAG_stackablesHauler;

	// The cooldown is mostly currently for spam limiting.
	// A good value for this is 0.001s (10ms). That blocks same frame activations but allows activations on individual frames;
	pub float cooldownS;

	// Disabled if "< 0";
	pub float pickupRangeM;
	// Disabled if "< 0";
	pub float throwRangeM;

	pub std::string* activationSoundConfig_id;
	pub float activationSoundConfig_volume;

	// <=0 not used.
	// >0 used.
	pub float throwPosXYGridSize;
	// <=0 not used.
	// >0 used.
	pub float throwAngDegZGridSize;

public:
	pub dCtor(ToolConfigStackablesHauler);
	pub explicit ToolConfigStackablesHauler(
		std::string id,
		int filtersPriority,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		float pickupRangeM,
		float throwRangeM,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume,
		float throwPosXYGridSize, float throwAngDegZGridSize);

	pub bool getCanPickup();
	pub bool getCanThrow();

	pub sp<Tool> createNewTool(IAppAssets* appAssets) override;

	pub ~ToolConfigStackablesHauler() override;
};
};
