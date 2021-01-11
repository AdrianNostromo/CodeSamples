#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class LivingEntityTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(LivingEntityTemplate);
	pub float maxHP;

    pub explicit LivingEntityTemplate(float maxHP);

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~LivingEntityTemplate() override;
};
};
