#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/cooldownTool/CooldownTool.h>

namespace rpg3D {
	class ProjectileConfig;
};

namespace rpg3D {
class AmmunitionCooldownTool : public CooldownTool {priv typedef CooldownTool super;pub dCtor(AmmunitionCooldownTool);
	priv ProjectileConfig* projectileConfig = nullptr;
	
	pub explicit AmmunitionCooldownTool(ToolConfigPersistentCooldown* config);

	pub ProjectileConfig* peekProjectileConfig();
	pub bool hasProjectileConfig();
	pub void setProjectileConfig(ProjectileConfig* projectileConfig);
	prot virtual void onProjectileConfigChanged();

	pub bool canStartTriggering() override;

    pub ~AmmunitionCooldownTool() override;
};
};
