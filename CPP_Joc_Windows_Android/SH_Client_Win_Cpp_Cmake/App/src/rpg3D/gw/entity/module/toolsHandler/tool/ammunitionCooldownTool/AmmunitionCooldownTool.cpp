#include "AmmunitionCooldownTool.h"
#include <rpg3D/gw/projectile/ProjectileConfig.h>

using namespace rpg3D;

AmmunitionCooldownTool::AmmunitionCooldownTool(ToolConfigPersistentCooldown* config)
	: super(config)
{
	//void
}

ProjectileConfig* AmmunitionCooldownTool::peekProjectileConfig() {
	return projectileConfig;
}

bool AmmunitionCooldownTool::hasProjectileConfig() {
	if (projectileConfig != nullptr) {
		return true;
	}

	return false;
}

void AmmunitionCooldownTool::setProjectileConfig(ProjectileConfig* projectileConfig) {
	this->projectileConfig = projectileConfig;

	onProjectileConfigChanged();
}

void AmmunitionCooldownTool::onProjectileConfigChanged() {
	//void
}

bool AmmunitionCooldownTool::canStartTriggering() {
	if (!super::canStartTriggering()) {
		return false;
	}

	if (!hasProjectileConfig()) {
		return false;
	}

	return true;
}

AmmunitionCooldownTool::~AmmunitionCooldownTool() {
	//void
}
