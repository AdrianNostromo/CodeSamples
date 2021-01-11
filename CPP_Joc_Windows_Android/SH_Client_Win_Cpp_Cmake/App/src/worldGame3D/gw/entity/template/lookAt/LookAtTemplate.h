#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <string>

class LookAtTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(LookAtTemplate);
    pub explicit LookAtTemplate();
    
    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;

    pub ~LookAtTemplate() override;
};
