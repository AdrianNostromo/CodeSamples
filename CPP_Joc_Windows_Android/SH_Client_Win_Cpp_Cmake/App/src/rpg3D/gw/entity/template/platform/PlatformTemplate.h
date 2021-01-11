#pragma once

#include <base/gh.h>
#include <string>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class PlatformTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(PlatformTemplate);
	pub explicit PlatformTemplate();
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

	pub ~PlatformTemplate() override;
};
};
