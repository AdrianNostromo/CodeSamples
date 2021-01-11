#include "PlatformInteractorModule.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>

using namespace rpg3D;

PlatformInteractorModule::PlatformInteractorModule(
    IWorldEntity* entity,
    PlatformInteractorTemplate* t)
    : super(t->moduleName, entity),
	t(t)
{
    //void
}

PlatformInteractorModule::~PlatformInteractorModule() {
    //void
}
