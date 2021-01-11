#pragma once

#include <base/gh.h>
#include <string>
#include <base/object/Object.h>
#include <memory>
#include <base/object/IWrappedValue.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffect.h>

class IWorldEntity;

namespace rpg3D {
class EffectPayload {pub dCtor(EffectPayload);
	pub class Types {
		pub static const int physicalDamage;
		pub static const int giveGoldAmount;
		pub static const int fixedValueHeal;
		pub static const int statusEffect;
	};
	pub class Target {
		pub static const int OTHER_ENTITY;
		pub static const int SELF_ENTITY;
		// Note. Can do area_of_effect targets also.
	};

	// Currently, don't edit these because they are kept as shared pointers. Need a copy mechanism before editing should be enabled.
	pub int type;

	pub int target;

	pub std::shared_ptr<base::IWrappedValue> value;

	pub explicit EffectPayload(int type, int target, float value);
	pub explicit EffectPayload(int type, int target, std::shared_ptr<CfgStatusEffect> statusEffectConfig);

	// valueMultipler is used to change damage/ heal value. It is currently used to scale down damage when multiple sword, projectile impacts occur.
	pub void transferToEntity(IWorldEntity* selfEntity, IWorldEntity* otherEntity, float valueMultipler = 1.0f);

	pub virtual ~EffectPayload();
};
};
