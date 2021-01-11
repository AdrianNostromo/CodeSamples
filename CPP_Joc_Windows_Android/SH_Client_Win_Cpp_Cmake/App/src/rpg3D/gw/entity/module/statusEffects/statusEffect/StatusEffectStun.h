#pragma once

#include "StatusEffect.h"
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>

class IMovingEntityModule;

namespace rpg3D {
class StatusEffectStun : public StatusEffect {
private: typedef StatusEffect super;
public:
	std::shared_ptr<CfgStatusEffectStun> config;

private:
	IMovingEntityModule* movingEntity = nullptr;

	bool isFirstTickProcessed = false;

public:
    StatusEffectStun(StatusEffectStun const&) = delete;
    StatusEffectStun(StatusEffectStun &&) = default;
    StatusEffectStun& operator=(StatusEffectStun const&) = delete;
    StatusEffectStun& operator=(StatusEffectStun &&) = default;

    explicit StatusEffectStun(std::shared_ptr<CfgStatusEffectStun> config, IWorldEntity* entity);

	void tick(float deltaS) override;

    ~StatusEffectStun() override;
};
};
