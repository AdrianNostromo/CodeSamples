#include <rpg3D/gw/entity/module/statusEffects/statusEffect/StatusEffectKnockback.h>
#include "CfgStatusEffectKnockback.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

int CfgStatusEffectKnockback::TYPE = super::NewTypeIndex();

CfgStatusEffectKnockback::CfgStatusEffectKnockback(float durationS, float distance, float jumpHeight)
	: super(TYPE, durationS),
	  distance(distance), jumpHeight(jumpHeight)
{
	//void
}

StatusEffect* CfgStatusEffectKnockback::buildSP(std::shared_ptr<CfgStatusEffect> statusEffectConfig, IWorldEntity* entity) {
	std::shared_ptr<CfgStatusEffectKnockback> cStatusEffectConfig = std::static_pointer_cast<CfgStatusEffectKnockback>(statusEffectConfig);
	if(cStatusEffectConfig == nullptr) {
		throw LogicException(LOC);
	}

	StatusEffectKnockback* effect = new StatusEffectKnockback(
		cStatusEffectConfig,
		entity
	);

	return static_cast<StatusEffect*>(effect);
}

CfgStatusEffectKnockback::~CfgStatusEffectKnockback() {
	//void
}
