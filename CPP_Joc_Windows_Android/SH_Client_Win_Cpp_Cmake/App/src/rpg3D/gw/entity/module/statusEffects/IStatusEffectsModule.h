#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <memory>

namespace rpg3D {
	class CfgStatusEffect;
};

namespace rpg3D {
class IStatusEffectsModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub static base::EntityEvent* EVENT_statusEffects_changed;

	pub virtual bool getIsEffectActive(int type) = 0;

	pub virtual void addAction_ActionDoStatusEffect(std::shared_ptr<CfgStatusEffect> effectConfig) = 0;

	pub ~IStatusEffectsModule() override = default;
};
};
