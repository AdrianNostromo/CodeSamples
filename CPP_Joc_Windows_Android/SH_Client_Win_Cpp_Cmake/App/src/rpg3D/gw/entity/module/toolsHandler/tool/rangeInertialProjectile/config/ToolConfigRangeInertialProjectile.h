#pragma once

#include <base/gh.h>
#include <base/visual3d/util/Visual3DCreateConfig.h>
#include <base/audio/sound/util/CfgSound.h>
#include "../../config/ToolConfigPersistentCooldown.h"
#include <base/memory/SharedPointer.h>

namespace rpg3D {
class ToolConfigRangeInertialProjectile : public ToolConfigPersistentCooldown {priv typedef ToolConfigPersistentCooldown super;pub dCtor(ToolConfigRangeInertialProjectile);
	pub class ProjectileSpawnConfig {pub dCtor(ProjectileSpawnConfig);
		pub std::string projectileSpawnParentId;

		pub float spawnAngZDeg;
		pub float maxSpawnAngZDegError;

		pub explicit ProjectileSpawnConfig(std::string projectileSpawnParentId, float spawnAngZDeg, float maxSpawnAngZDegError)
			: projectileSpawnParentId(projectileSpawnParentId), spawnAngZDeg(spawnAngZDeg), maxSpawnAngZDegError(maxSpawnAngZDegError)
		{}
	};

	pub static std::string TAG_weapon;
	pub static std::string TAG_bow;
	pub static std::string TAG_range;

	pub float projectileLaunchPhysicsImpulse;

	pub sp<std::string> projectilePreviewParentId;

	pub Visual3DCreateConfig* toolVisualCreateConfig;

	pub std::string* activationSoundConfig_id;
	pub float activationSoundConfig_volume;

	pub ArrayList<ProjectileSpawnConfig> projectileSpawnConfigsList{};

	pub explicit ToolConfigRangeInertialProjectile(
		std::string id,
		int filtersPriority, 
		bool supportsContinuousReactivation,
		std::shared_ptr<std::string> icon_atlasId, std::shared_ptr<std::string> icon_regionId,
		std::string shortName_partA, std::string shortName_partB,
		float cooldownS, float channelingDurationS,
		float projectileLaunchPhysicsImpulse,
		sp<std::string> projectilePreviewParentId,
		Visual3DCreateConfig* toolVisualCreateConfig,
		std::string* activationSoundConfig_id, float activationSoundConfig_volume);

	pub sp<Tool> createNewTool(IAppAssets* appAssets) override;

	pub ~ToolConfigRangeInertialProjectile() override;
};
};
