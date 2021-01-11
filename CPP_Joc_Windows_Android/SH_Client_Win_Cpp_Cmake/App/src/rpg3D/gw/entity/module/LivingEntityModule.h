#pragma once

#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "ILivingEntityModule.h"
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

namespace rpg3D {
class LivingEntityModule : public EntityModule, public virtual ILivingEntityModule {priv typedef EntityModule super;pub dCtor(LivingEntityModule);
    pub static EntityRemoveReason *const EER_LIVING_ENTITY_DESTROYED;

	pub static base::EntityEvent* EVENT_Common_currentHP_Change;
	pub static base::EntityEvent* EVENT_livingEntity_ZeroHp;

	pub LivingEntityTemplate* t;

    // The initial hp value is initialised locally to the max hp.
	priv float currentHP = 0;

	priv bool removeEntityFromGWOnZeroHp = true;

	pub explicit LivingEntityModule(
	    IWorldEntity* entity,
	    LivingEntityTemplate* t);

	pub LivingEntityTemplate* getT() final;
	pub LivingEntityTemplate* getTemplate() final;

	pub float getCurrentHP() final;
	pub void setCurrentHP(float currentHP) final;
	pub void addCurrentHP(float hpAddAmount) final;

	pub bool getRemoveEntityFromGWOnZeroHp() final;
	pub void setRemoveEntityFromGWOnZeroHp(bool removeEntityFromGWOnZeroHp) final;

	pub ~LivingEntityModule() override;
};
};
