#include "ActionsModule.h"
#include <base/exceptions/LogicException.h>

using namespace worldGame3D;

ActionsModule::ActionsModule(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(entity, t)
{
    //void
}

ActionsModule::~ActionsModule() {
	//void
}
