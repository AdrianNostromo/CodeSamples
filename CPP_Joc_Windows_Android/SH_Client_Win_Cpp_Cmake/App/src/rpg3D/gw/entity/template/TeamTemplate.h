#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class TeamTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(TeamTemplate);
    pub explicit TeamTemplate();

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~TeamTemplate() override;
};
};
