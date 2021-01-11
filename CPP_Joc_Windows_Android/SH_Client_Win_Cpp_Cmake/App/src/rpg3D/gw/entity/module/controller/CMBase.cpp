#include "CMBase.h"

using namespace rpg3D;

CMBase::CMBase(
	IWorldEntity* entity,
	ControllerTemplate* t)
	: super(t->moduleName, entity),
	  t(t)
{
	//void
}

CMBase::~CMBase() {
	//void
}
