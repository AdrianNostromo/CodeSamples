#include "P3DM_Base.h"
#include "../../template/physics3D/Physics3DTemplate.h"

using namespace rpg3D;

P3DM_Base::P3DM_Base(
	IWorldEntity* entity,
	Physics3DTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

void P3DM_Base::createBMain() {
	super::createBMain();

	//void
}

P3DM_Base::~P3DM_Base() {
	//void
}
