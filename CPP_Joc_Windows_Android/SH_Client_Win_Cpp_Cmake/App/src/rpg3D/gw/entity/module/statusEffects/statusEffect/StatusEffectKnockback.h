#pragma once

#include "StatusEffect.h"
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectKnockback.h>

namespace rpg3D {
class StatusEffectKnockback : public StatusEffect {
private: typedef StatusEffect super;
public:
	std::shared_ptr<CfgStatusEffectKnockback> config;

	float lastTick_cpDist = 0.0f;
	float lastTick_cpPosOffsetZ = 0.0f;

public:
    StatusEffectKnockback(StatusEffectKnockback const&) = delete;
    StatusEffectKnockback(StatusEffectKnockback &&) = default;
    StatusEffectKnockback& operator=(StatusEffectKnockback const&) = delete;
    StatusEffectKnockback& operator=(StatusEffectKnockback &&) = default;

    explicit StatusEffectKnockback(std::shared_ptr<CfgStatusEffectKnockback> config, IWorldEntity* entity);

	void tick(float deltaS) override;

    ~StatusEffectKnockback() override;
};
};
