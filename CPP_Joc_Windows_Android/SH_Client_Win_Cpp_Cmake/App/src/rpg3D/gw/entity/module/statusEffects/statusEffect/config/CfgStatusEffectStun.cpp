#include "CfgStatusEffectStun.h"
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/StatusEffectStun.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

int CfgStatusEffectStun::TYPE = super::NewTypeIndex();

CfgStatusEffectStun::CfgStatusEffectStun(float durationS)
	: super(TYPE, durationS)
{
	//void
}

StatusEffect* CfgStatusEffectStun::buildSP(std::shared_ptr<CfgStatusEffect> statusEffectConfig, IWorldEntity* entity) {
	std::shared_ptr<CfgStatusEffectStun> cStatusEffectConfig = std::static_pointer_cast<CfgStatusEffectStun>(statusEffectConfig);
	if(cStatusEffectConfig == nullptr) {
		throw LogicException(LOC);
	}

	StatusEffectStun* effect = new StatusEffectStun(
		cStatusEffectConfig,
		entity
	);

	return static_cast<StatusEffect*>(effect);
}

CfgStatusEffectStun::~CfgStatusEffectStun() {
	//void
}
