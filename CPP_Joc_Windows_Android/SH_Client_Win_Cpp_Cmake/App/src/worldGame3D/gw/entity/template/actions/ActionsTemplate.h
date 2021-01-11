#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace worldGame3D {
class ActionsTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(ActionsTemplate);
    pub explicit ActionsTemplate();

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~ActionsTemplate() override;
};
};
