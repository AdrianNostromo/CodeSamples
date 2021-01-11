#pragma once

#include "IStatusEffect.h"
#include <memory>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffect.h>
#include <base/list/ListDL.h>

class IWorldEntity;

namespace rpg3D {
class StatusEffect : virtual public IStatusEffect {
public:
	std::shared_ptr<CfgStatusEffect> config;

	IWorldEntity* entity;
	IWorldEntity* e;

	float lifeS = 0.0f;

	int dependantTrackingId = -1;
	bool isDependantTrackingParent = false;

	StatusEffect* dependantTrackingParentEffect = nullptr;
	ArrayList<StatusEffect*> dependantTrackingChildEffectsList{};

	// List entry pointer for easier dependant status_effects removal.
	ListDL<StatusEffect*>::Entry* listEntry = nullptr;

public:
    StatusEffect(StatusEffect const&) = delete;
    StatusEffect(StatusEffect &&) = default;
    StatusEffect& operator=(StatusEffect const&) = delete;
    StatusEffect& operator=(StatusEffect &&) = default;

    explicit StatusEffect(std::shared_ptr<CfgStatusEffect> config, IWorldEntity* entity);

    virtual void tick(float deltaS);

    ~StatusEffect() override;
};
};
