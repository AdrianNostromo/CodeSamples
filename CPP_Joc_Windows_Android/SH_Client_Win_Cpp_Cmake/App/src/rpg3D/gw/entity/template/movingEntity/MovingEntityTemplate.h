#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/BasicModuleTemplate.h>
#include <base/list/ArrayList.h>

namespace rpg3D {
    class MovementConfig;
};

namespace rpg3D {
class MovingEntityTemplate : public BasicModuleTemplate {priv typedef BasicModuleTemplate super;pub dCtor(MovingEntityTemplate);
    pub float turnZSpeedDegS_min;
    pub float turnZSpeedDegS_max;

    priv ArrayList<rpg3D::MovementConfig*> movementConfigsList{};

    pub MovingEntityTemplate(
        float turnZSpeedDegS_min, float turnZSpeedDegS_max
    );

    pub IEntityModule* createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) override;
    
    pub void pushMovementConfig(rpg3D::MovementConfig* movementConfig);
    pub rpg3D::MovementConfig* getDefaultMovementConfigMustExist();
    pub rpg3D::MovementConfig* getMovementConfigOptional(int type);
    pub rpg3D::MovementConfig* getMovementConfigMustExist(int type);

    pub ~MovingEntityTemplate() override;
};
};
