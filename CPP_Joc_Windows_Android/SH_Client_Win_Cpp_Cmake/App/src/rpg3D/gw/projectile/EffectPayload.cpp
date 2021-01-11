#include "EffectPayload.h"
#include <base/object/WrappedValue.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/statusEffects/StatusEffectsModule.h>
#include <rpg3D/gw/entity/module/inventory/IInventoryModule.h>
#include <base/math/Math.h>

using namespace rpg3D;

const int EffectPayload::Types::physicalDamage = 1;
const int EffectPayload::Types::giveGoldAmount = 2;
const int EffectPayload::Types::fixedValueHeal = 3;
const int EffectPayload::Types::statusEffect = 4;

const int EffectPayload::Target::OTHER_ENTITY = 1;
const int EffectPayload::Target::SELF_ENTITY = 2;

EffectPayload::EffectPayload(int type, int target, float value)
	: type(type), target(target)
{
	if (type != EffectPayload::Types::physicalDamage 
		&& type != EffectPayload::Types::giveGoldAmount 
		&& type != EffectPayload::Types::fixedValueHeal)
	{
		// Payload is not of float type. Add exception when int payload exists.
		throw LogicException(LOC);
	}

	this->value = base::IWrappedValue::new_float(value);
}

EffectPayload::EffectPayload(int type, int target, std::shared_ptr<CfgStatusEffect> statusEffectConfig)
	: type(type), target(target)
{
	this->value = base::IWrappedValue::new_Custom(statusEffectConfig, -1);
}

void EffectPayload::transferToEntity(IWorldEntity* selfEntity, IWorldEntity* otherEntity, float valueMultipler) {
	if(selfEntity == nullptr || otherEntity == nullptr) {
		throw LogicException(LOC);
	}

	// Transfer payload to the target.
	IWorldEntity* payloadTargetEntity;
	if (target == EffectPayload::Target::OTHER_ENTITY) {
		payloadTargetEntity = otherEntity;
	}else if (target == EffectPayload::Target::SELF_ENTITY) {
		payloadTargetEntity = selfEntity;
	}else {
		throw LogicException(LOC);
	}

	if (type == EffectPayload::Types::physicalDamage) {
		float damageAmount = value->getDirectAs_float();

		payloadTargetEntity->getComponentAs<ILivingEntityModule*>(true/*mustExist*/)
			->addCurrentHP(-damageAmount * valueMultipler);
	}else if (type == EffectPayload::Types::statusEffect) {
		std::shared_ptr<CfgStatusEffect> effectConfig = value->getDirectAs_Custom<std::shared_ptr<CfgStatusEffect>>(-1);

		payloadTargetEntity->getComponentAs<IStatusEffectsModule*>(true/*mustExist*/)
			->addAction_ActionDoStatusEffect(effectConfig);
	}else if (type == EffectPayload::Types::giveGoldAmount) {
		rpg3D::IInventoryModule* oInventory = payloadTargetEntity->getComponentAs<rpg3D::IInventoryModule*>(true/*mustExist*/);

		float goldAmount = value->getDirectAs_float();

		oInventory->addGoldAmount(goldAmount * valueMultipler);
	}else if (type == EffectPayload::Types::fixedValueHeal) {
		float healAmount = value->getDirectAs_float();

		payloadTargetEntity->getComponentAs<ILivingEntityModule*>(true/*mustExist*/)
			->addCurrentHP(healAmount * valueMultipler);
	}
	
	else {
		throw LogicException(LOC);
	}
}

EffectPayload::~EffectPayload() {
	//void
}
