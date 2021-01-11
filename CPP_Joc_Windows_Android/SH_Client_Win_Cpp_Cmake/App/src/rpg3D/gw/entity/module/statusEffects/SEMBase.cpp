#include "SEMBase.h"

using namespace rpg3D;

SEMBase::SEMBase(
	IWorldEntity* entity,
	StatusEffectsTemplate* t)
	: super(t->moduleName, entity),
	  t(t)
{
	//void
}

SEMBase::~SEMBase() {
	//void
}
