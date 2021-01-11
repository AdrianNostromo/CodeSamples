#include "ProjectileModule.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>

using namespace rpg3D;

ProjectileModule::ProjectileModule(
    IWorldEntity* entity,
    ProjectileTemplate* t)
    : super(entity, t)
{
    //void
}

ProjectileModule::~ProjectileModule() {
	//void
}

