#pragma once

#include "CfgStatusEffect.h"

namespace rpg3D {
class CfgStatusEffectKnockback : public CfgStatusEffect {
private: typedef CfgStatusEffect super;
public:
	static int TYPE;

	float distance;
	float jumpHeight;

public:
    CfgStatusEffectKnockback(CfgStatusEffectKnockback const&) = delete;
    CfgStatusEffectKnockback(CfgStatusEffectKnockback &&) = default;
    CfgStatusEffectKnockback& operator=(CfgStatusEffectKnockback const&) = delete;
    CfgStatusEffectKnockback& operator=(CfgStatusEffectKnockback &&) = default;

    explicit CfgStatusEffectKnockback(float durationS, float distance, float jumpHeight);

	StatusEffect* buildSP(std::shared_ptr<CfgStatusEffect> statusEffectConfig, IWorldEntity* entity) override;

	~CfgStatusEffectKnockback() override;
};
};
