#pragma once

#include "CfgStatusEffect.h"

namespace rpg3D {
class CfgStatusEffectStun : public CfgStatusEffect {
private: typedef CfgStatusEffect super;
public:
	static int TYPE;

public:
    CfgStatusEffectStun(CfgStatusEffectStun const&) = delete;
    CfgStatusEffectStun(CfgStatusEffectStun &&) = default;
    CfgStatusEffectStun& operator=(CfgStatusEffectStun const&) = delete;
    CfgStatusEffectStun& operator=(CfgStatusEffectStun &&) = default;

    explicit CfgStatusEffectStun(float durationS);

	StatusEffect* buildSP(std::shared_ptr<CfgStatusEffect> statusEffectConfig, IWorldEntity* entity) override;

    ~CfgStatusEffectStun() override;
};
};
