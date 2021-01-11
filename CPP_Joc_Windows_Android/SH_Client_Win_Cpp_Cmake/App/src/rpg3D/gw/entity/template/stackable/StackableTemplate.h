#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class StackableTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super; pub dCtor(StackableTemplate);
	pub bool canHaul;
	pub bool canBeCarried;

	pub float haulOffsetZM;

	pub bool allowPlaceOnThis;

	pub explicit StackableTemplate(
		bool canHaul, bool canBeCarried,
		float haulOffsetZM,
		bool allowPlaceOnThis);

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

    pub ~StackableTemplate() override;
};
};
