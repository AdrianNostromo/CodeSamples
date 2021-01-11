#include "StatusEffectsModule.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>

using namespace rpg3D;

StatusEffectsModule::StatusEffectsModule(
    IWorldEntity* entity,
    StatusEffectsTemplate* t)
    : super(entity, t)
{
	//void
}

StatusEffectsModule::~StatusEffectsModule() {
	//void
}
