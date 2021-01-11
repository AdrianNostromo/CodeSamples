#include "G2DCM_Base.h"

using namespace rpg3D;

G2DCM_Base::G2DCM_Base(
	IWorldEntity* entity,
	Grid2DContentTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	//void
}

G2DCM_Base::~G2DCM_Base() {
	//void
}
