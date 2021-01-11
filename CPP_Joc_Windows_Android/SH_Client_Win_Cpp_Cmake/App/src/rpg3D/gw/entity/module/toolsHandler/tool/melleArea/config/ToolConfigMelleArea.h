#pragma once

#include <base/gh.h>
#include <base/visual3d/util/Visual3DCreateConfig.h>
#include <base/audio/sound/util/CfgSound.h>
#include "../../config/ToolConfigPersistentCooldown.h"

namespace rpg3D {
	class PBodyConfig;
};

namespace rpg3D {
class ToolConfigMelleArea : public ToolConfigPersistentCooldown {priv typedef ToolConfigPersistentCooldown super;pub dCtor(ToolConfigMelleArea);
	pub static std::string TAG_weapon;
	pub static std::string TAG_sword;
	pub static std::string TAG_melle;

	pub enum ImpactAcknowledgeType { EachContact, OnlyFirstContact };

	pub ImpactAcknowledgeType impactAcknowledgeType;
	//asd_1;// implement in code;
	pub float reImpactOnPersistentContactCooldownS;

	pub Visual3DCreateConfig* toolVisualCreateConfig;

	pub std::string* activationSoundConfig_id;
	pub float activationSoundConfig_volume;
	
	pub std::string* impactSoundConfig_id;
	pub float impactSoundConfig_volume;
	pub float impactSoundConfig_overlapCooldownS;

	// managed
	pub rpg3D::PBodyConfig* triggerPBodyConfig;

	pub float triggerDurationS;

	pub Vector3 trigger_pBody_pos_start;
	pub Vector3 trigger_pBody_pos_end;

	pub float trigger_pBody_scale_start;
	pub float trigger_pBody_scale_end;

	// < 0; un-capped.
	// >= 0; Limit exists.
	// A value of 12 seems safe.
	pub int maxTargetImpactsCount;
	// eg. 0.1f will decreate the payload values by 0.1f for each impact until the minimum is reached.
	pub float payloadPercentDecreasePerTargetImpact;
	// The above multiplier is clipped by this.
	// eg. 0.2f will make the minimum payload multiplier be 0.2f;
	pub float minPayloadsValuesMultiplier;

	pub explicit ToolConfigMelleArea(
		std::string id,
		int filtersPriority, 
		bool supportsContinuousReactivation,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		ImpactAcknowledgeType impactAcknowledgeType, float reImpactOnPersistentContactCooldownS,
		Visual3DCreateConfig* toolVisualCreateConfig,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume,
		std::string* impactSoundConfig_id, float impactSoundConfig_volume, float impactSoundConfig_overlapCooldownS,
		rpg3D::PBodyConfig* triggerPBodyConfig,
		float triggerDurationS,
		const Vector3 trigger_pBody_pos_start, const Vector3 trigger_pBody_pos_end,
		float trigger_pBody_scale_start, float trigger_pBody_scale_end,
		int maxTargetImpactsCount, float payloadPercentDecreasePerTargetImpact, float minPayloadsValuesMultiplier);

	pub sp<Tool> createNewTool(IAppAssets* appAssets) override;

	pub ~ToolConfigMelleArea() override;
};
};
