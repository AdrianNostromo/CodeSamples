#include "FM_Base.h"
#include "../../template/flying/FlyingTemplate.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>

using namespace rpg3D;

FM_Base::FM_Base(
	IWorldEntity* entity,
	FlyingTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

void FM_Base::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(false/*mustExist*/);
}

FM_Base::~FM_Base() {
	//void
}
