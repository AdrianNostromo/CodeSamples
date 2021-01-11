#include "LivingEntityModule.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/math/Math.h>

using namespace rpg3D;

EntityRemoveReason *const LivingEntityModule::EER_LIVING_ENTITY_DESTROYED = new EntityRemoveReason("EER_LIVING_ENTITY_DESTROYED", true, true);

base::EntityEvent* LivingEntityModule::EVENT_Common_currentHP_Change = new base::EntityEvent(false, "EVENT_Common_currentHP_Change");
base::EntityEvent* LivingEntityModule::EVENT_livingEntity_ZeroHp = new base::EntityEvent(false, "EVENT_livingEntity_ZeroHp");

LivingEntityModule::LivingEntityModule(
    IWorldEntity* entity,
    LivingEntityTemplate* t)
    : super(t->moduleName, entity),
      t(t)
{
	currentHP = this->t->maxHP;
}

LivingEntityTemplate* LivingEntityModule::getT() {
    return t;
}

LivingEntityTemplate* LivingEntityModule::getTemplate() {
    return t;
}

float LivingEntityModule::getCurrentHP() {
    return currentHP;
}

void LivingEntityModule::setCurrentHP(float currentHP) {
	currentHP = Math::max(0.0f, Math::min(t->maxHP, currentHP));

    if(this->currentHP == currentHP) {
        return ;
    }

    this->currentHP = currentHP;

    e->dispatchEvent(*EVENT_Common_currentHP_Change);

    if(this->currentHP <= 0) {
        // This entity was destroyed, mark it for removal.
		if (removeEntityFromGWOnZeroHp) {
			entity->getHandler()->markEntityForRemoval(entity, EER_LIVING_ENTITY_DESTROYED, false);
		}
		
        e->dispatchEvent(*EVENT_livingEntity_ZeroHp);
    }
}

void LivingEntityModule::addCurrentHP(float hpAddAmount) {
    setCurrentHP(currentHP + hpAddAmount);
}

bool LivingEntityModule::getRemoveEntityFromGWOnZeroHp() {
	return removeEntityFromGWOnZeroHp;
}

void LivingEntityModule::setRemoveEntityFromGWOnZeroHp(bool removeEntityFromGWOnZeroHp) {
	this->removeEntityFromGWOnZeroHp = removeEntityFromGWOnZeroHp;
}

LivingEntityModule::~LivingEntityModule() {
	//void
}
