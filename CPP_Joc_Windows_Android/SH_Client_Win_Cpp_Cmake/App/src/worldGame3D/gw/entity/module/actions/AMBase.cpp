#include "AMBase.h"

using namespace worldGame3D;

AMBase::AMBase(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(t->moduleName, entity),
	  t(t)
{
	//void
}

AMBase::~AMBase() {
	//void
}
