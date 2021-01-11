#include "MovingEntityTemplate.h"
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <base/exceptions/LogicException.h>
#include "movementConfig/MovementConfig.h"

using namespace rpg3D;

MovingEntityTemplate::MovingEntityTemplate(
	float turnZSpeedDegS_min, float turnZSpeedDegS_max)
	: super("movingEntity", IMovingEntityModule::TYPE),
	turnZSpeedDegS_min(turnZSpeedDegS_min), turnZSpeedDegS_max(turnZSpeedDegS_max)
{
    //void
}

IEntityModule* MovingEntityTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    MovingEntityTemplate* t = dynamic_cast<MovingEntityTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    MovingEntityModule* mod = new MovingEntityModule(entity, t);

    return mod;
}

void MovingEntityTemplate::pushMovementConfig(rpg3D::MovementConfig* movementConfig) {
    if (getMovementConfigOptional(movementConfig->getType()) != nullptr) {
        // Movement type already exists.
        throw LogicException(LOC);
    }

    movementConfigsList.appendDirect(movementConfig);
}

rpg3D::MovementConfig* MovingEntityTemplate::getDefaultMovementConfigMustExist() {
    rpg3D::MovementConfig* mConfig = movementConfigsList.getDirect(0, nullptr);
    if (mConfig == nullptr) {
        // No movement config exists.
        throw LogicException(LOC);
    }

    return mConfig;
}

rpg3D::MovementConfig* MovingEntityTemplate::getMovementConfigOptional(int type) {
    for (int i = 0; i < movementConfigsList.count(); i++) {
        rpg3D::MovementConfig* mConfig = movementConfigsList.getDirect(i);

        if (mConfig->getType() == type) {
            return mConfig;
        }
    }

    return nullptr;
}

rpg3D::MovementConfig* MovingEntityTemplate::getMovementConfigMustExist(int type) {
    for (int i = 0; i < movementConfigsList.count(); i++) {
        rpg3D::MovementConfig* mConfig = movementConfigsList.getDirect(i);

        if (mConfig->getType() == type) {
            return mConfig;
        }
    }

    throw LogicException(LOC);
}

MovingEntityTemplate::~MovingEntityTemplate() {
    //void
}
