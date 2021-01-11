#include "CfgStatusEffect.h"

using namespace rpg3D;

int CfgStatusEffect::TYPE_COUNTER = 0;

int CfgStatusEffect::NewTypeIndex() {
	return ++TYPE_COUNTER;
}

CfgStatusEffect::CfgStatusEffect(int type, float durationS)
	: type(type), durationS(durationS)
{
	//void
}

void CfgStatusEffect::addSubEffect(std::shared_ptr<CfgStatusEffect> subStatusEffectConfig, bool isDependantTracked) {
	subStatusEffectsList.append_emplace(subStatusEffectConfig, isDependantTracked);
}

CfgStatusEffect::~CfgStatusEffect() {
	//void
}
