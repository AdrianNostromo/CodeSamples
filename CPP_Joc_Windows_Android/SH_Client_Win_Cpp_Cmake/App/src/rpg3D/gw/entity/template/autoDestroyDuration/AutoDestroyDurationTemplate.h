#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>

namespace rpg3D {
class AutoDestroyDurationTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(AutoDestroyDurationTemplate);
    pub float lifeDistM;
    pub float lifeDurationS;

    pub AutoDestroyDurationTemplate(
        float lifeDistM, float lifeDurationS
    );
    
	pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
	pub ~AutoDestroyDurationTemplate() override;
};
};
