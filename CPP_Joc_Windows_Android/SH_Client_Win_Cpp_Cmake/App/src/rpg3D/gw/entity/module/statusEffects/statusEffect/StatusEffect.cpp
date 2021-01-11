#include "StatusEffect.h"

using namespace rpg3D;

StatusEffect::StatusEffect(std::shared_ptr<CfgStatusEffect> config, IWorldEntity* entity)
	: config(config), entity(entity), e(entity)
{
	//void
}

void StatusEffect::tick(float deltaS) {
	lifeS += deltaS;
}

StatusEffect::~StatusEffect() {
	//void
}
