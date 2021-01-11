#include "SEMActiveEffects.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

SEMActiveEffects::SEMActiveEffects(
	IWorldEntity* entity,
	StatusEffectsTemplate* t)
	: super(entity, t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&SEMActiveEffects::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void SEMActiveEffects::addStatusEffect(
	std::shared_ptr<CfgStatusEffect> effectConfig,
	int dependantTrackingId, bool isDependantTrackingParent)
{
	StatusEffect* statusEffect = effectConfig->buildSP(effectConfig, entity);

	ListDL<StatusEffect*>::Entry* listEntry = activeStatusEffectsList.appendReference(statusEffect);
	// Save the list entry pointer in statusEffect for easier dependant status_effects removal.
	statusEffect->listEntry = listEntry;

	if(dependantTrackingId >= 0) {
		statusEffect->dependantTrackingId = dependantTrackingId;
		statusEffect->isDependantTrackingParent = isDependantTrackingParent;

		if (statusEffect->isDependantTrackingParent) {
			// Set the entry on the map (must not already exist).
			if(dependantTrackingParentsMap.containsKey(statusEffect->dependantTrackingId)) {
				// Parent already in the map.
				throw LogicException(LOC);
			}
			dependantTrackingParentsMap.putDirect(statusEffect->dependantTrackingId, statusEffect);
		}else {
			// Get The parent entry from the map (must exist).
			StatusEffect* parentStatusEffect = dependantTrackingParentsMap.getDirect(statusEffect->dependantTrackingId, nullptr);
			if(parentStatusEffect == nullptr) {
				throw LogicException(LOC);
			}

			// Add statusEffect to parent.dependantTrackingChildEffectsList.
			parentStatusEffect->dependantTrackingChildEffectsList.appendDirect(statusEffect);

			// Set statusEffect.dependantTrackingParentEffect to the parent.
			statusEffect->dependantTrackingParentEffect = parentStatusEffect;
		}
	}

	incrementEffectCounter(effectConfig->type);
}

void SEMActiveEffects::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	auto lambdaFunc = [this, &event](ListDL<StatusEffect*>::Entry* listEntry, StatusEffect*& statusEffect, bool& stopEnumeration) {
		statusEffect->tick(event.deltaS);
		if(statusEffect->config->durationS >= 0 && statusEffect->lifeS >= statusEffect->config->durationS) {
			removeStatusEffect(listEntry);
		}
	};

	activeStatusEffectsList.enumerateReference(lambdaFunc);
}

bool SEMActiveEffects::getIsEffectActive(int type) {
	if(type < activeEffectCountersList.size()) {
		int val = activeEffectCountersList.getDirect(type);
		if(val > 0) {
			return true;
		}
	}

	return false;
}

void SEMActiveEffects::removeStatusEffect(ListDL<StatusEffect*>::Entry* listEntry) {
	StatusEffect* statusEffect = listEntry->data;

	if(statusEffect->dependantTrackingId >= 0) {
		if(statusEffect->isDependantTrackingParent) {
			// Fetch all child effects and remove them.
			while(statusEffect->dependantTrackingChildEffectsList.count() > 0) {
				// The removal from the list occurs in removeStatusEffect call below.
				StatusEffect* childStatusEffect = statusEffect->dependantTrackingChildEffectsList.getDirect(statusEffect->dependantTrackingChildEffectsList.count() - 1);

				removeStatusEffect(childStatusEffect->listEntry);
			}
		}else {
			// Remove statusEffect from the parent list.
			if(!statusEffect->dependantTrackingParentEffect->dependantTrackingChildEffectsList.remove(statusEffect)) {
				throw LogicException(LOC);
			}
			statusEffect->dependantTrackingParentEffect = nullptr;
		}
	}

	int statusEffectType = statusEffect->config->type;

	delete statusEffect;

	listEntry->remove();

	decrementEffectCounter(statusEffectType);
}

void SEMActiveEffects::incrementEffectCounter(int type) {
	while(activeEffectCountersList.size() <= type) {
		activeEffectCountersList.append_emplace(0);
	}

	int val = activeEffectCountersList.getDirect(type);
	activeEffectCountersList.setDirect(type, val + 1);

	e->dispatchEvent(*EVENT_statusEffects_changed);
}

void SEMActiveEffects::decrementEffectCounter(int type) {
	if(type >= activeEffectCountersList.size()) {
		throw LogicException(LOC);
	}

	int val = activeEffectCountersList.getDirect(type);
	if(val <= 0) {
		throw LogicException(LOC);
	}
	activeEffectCountersList.setDirect(type, val - 1);

	e->dispatchEvent(*EVENT_statusEffects_changed);
}

void SEMActiveEffects::disposeMain() {
	// Dispose the status effects from activeStatusEffectsList.
	while(activeStatusEffectsList.count() > 0) {
		removeStatusEffect(activeStatusEffectsList.getFirst());
	}

	super::disposeMain();
}

SEMActiveEffects::~SEMActiveEffects() {
	//void
}
