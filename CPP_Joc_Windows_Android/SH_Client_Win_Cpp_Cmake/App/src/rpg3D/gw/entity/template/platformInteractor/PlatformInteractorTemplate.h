#pragma once

#include <base/gh.h>
#include <string>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class PlatformInteractorTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(PlatformInteractorTemplate);
	pub explicit PlatformInteractorTemplate();

	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
	pub ~PlatformInteractorTemplate() override;
};
};
