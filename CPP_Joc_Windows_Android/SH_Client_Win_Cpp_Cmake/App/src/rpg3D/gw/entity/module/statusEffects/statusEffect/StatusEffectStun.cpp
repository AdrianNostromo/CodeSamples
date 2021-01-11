#include "StatusEffectStun.h"
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

StatusEffectStun::StatusEffectStun(std::shared_ptr<CfgStatusEffectStun> config, IWorldEntity* entity)
	: super(config, entity),
	  config(config)
{
	movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
}

void StatusEffectStun::tick(float deltaS) {
	super::tick(deltaS);

	if(!isFirstTickProcessed) {
		isFirstTickProcessed = true;

		movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);
	}
}

StatusEffectStun::~StatusEffectStun() {
	//void
}
