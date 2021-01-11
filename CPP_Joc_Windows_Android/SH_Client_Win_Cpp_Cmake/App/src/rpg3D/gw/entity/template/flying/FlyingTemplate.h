#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class FlyingTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(FlyingTemplate);
    pub float flyingSpeedS;

	pub FlyingTemplate(
        float flyingSpeedS
    );
    
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
	
	pub ~FlyingTemplate() override;
};
};
