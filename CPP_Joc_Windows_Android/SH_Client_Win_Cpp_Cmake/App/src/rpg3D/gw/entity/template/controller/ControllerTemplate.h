#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class ControllerTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(ControllerTemplate);
	pub explicit ControllerTemplate();

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~ControllerTemplate() override;
};
};
