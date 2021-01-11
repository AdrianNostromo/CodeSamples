#pragma once

#include <base/gh.h>
#include "SEMBase.h"
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffect.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include <base/util/PointersDoublyLinkedList.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/StatusEffect.h>
#include <base/list/ListDL.h>
#include <base/MM.h>

namespace rpg3D {
class SEMActiveEffects : public SEMBase {priv typedef SEMBase super;pub dCtor(SEMActiveEffects);
	priv ListDL<StatusEffect*> activeStatusEffectsList{LOC};

	priv ArrayList<int> activeEffectCountersList{};

	priv Map1D<int, StatusEffect*> dependantTrackingParentsMap{};

    pub explicit SEMActiveEffects(
	    IWorldEntity* entity,
	    StatusEffectsTemplate* t);

	pub void addStatusEffect(
		std::shared_ptr<CfgStatusEffect> effectConfig,
		int dependantTrackingId, bool isDependantTrackingParent);

	pub bool getIsEffectActive(int type) final;

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

	priv void removeStatusEffect(ListDL<StatusEffect*>::Entry* listEntry);

	priv void incrementEffectCounter(int type);
	priv void decrementEffectCounter(int type);

	pub void disposeMain() override;
	pub ~SEMActiveEffects() override;
};
};
