#include "CustomLogicModule.h"
#include <worldGame3D/gw/entity/template/customLogic/CustomLogicTemplate.h>

using namespace base;

CustomLogicModule::CustomLogicModule(
    IWorldEntity* entity,
	CustomLogicTemplate* t
)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

CustomLogicModule::~CustomLogicModule() {
    //void
}
