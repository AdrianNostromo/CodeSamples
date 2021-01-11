#include "StatusEffectKnockback.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>

using namespace rpg3D;

StatusEffectKnockback::StatusEffectKnockback(std::shared_ptr<CfgStatusEffectKnockback> config, IWorldEntity* entity)
	: super(config, entity),
	config(config)
{
	//void
}

void StatusEffectKnockback::tick(float deltaS) {
	super::tick(deltaS);

	float cp = Math::clamp(lifeS / config->durationS, 0.0f, 1.0f);
	float cpDist = Ease::computeEaseValue(
		Ease::easeOutCubic,
		cp,
		0.0f, config->distance
	);

	float deltaDist = cpDist - lastTick_cpDist;
	lastTick_cpDist = cpDist;

	float jump_partA_cp = Math::clamp(cp * 2.0f, 0.0f, 1.0f);
	float jump_partB_cp = Math::clamp((cp - 0.5f) * 2.0f, 0.0f, 1.0f);
	float cpPosOffsetZ = -jump_partA_cp * config->jumpHeight + jump_partB_cp * config->jumpHeight;
	float deltaPosOffsetZ = lastTick_cpPosOffsetZ - cpPosOffsetZ;
	lastTick_cpPosOffsetZ = cpPosOffsetZ;

	float rotZDeg = e->getRot()->getAngleAround(Vector3::Z);
	Vector3* pos = e->getPos();
	e->setPos(
		pos->x + Math::cosDegToRad(rotZDeg + 180.0f) * deltaDist,
		pos->y + Math::sinDegToRad(rotZDeg + 180.0f) * deltaDist,
		pos->z + deltaPosOffsetZ
	);
}

StatusEffectKnockback::~StatusEffectKnockback() {
	//void
}
