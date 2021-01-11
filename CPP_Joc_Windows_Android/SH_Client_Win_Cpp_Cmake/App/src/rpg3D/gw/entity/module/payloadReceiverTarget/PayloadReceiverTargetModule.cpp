#include "PayloadReceiverTargetModule.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/math/Math.h>
#include <base/math/Matrix4.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/template/payloadReceiverTarget/PayloadReceiverTargetTemplate.h>

using namespace rpg3D;

//asdA2;// If possible, optimise the bounds computation method and always compute it on pos change. That will be used for entities organising in a world grid.

PayloadReceiverTargetModule::PayloadReceiverTargetModule(
	IWorldEntity* entity,
	PayloadReceiverTargetTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	//void
}

PayloadReceiverTargetTemplate* PayloadReceiverTargetModule::getTemplate() {
	return t;
}

ITeamModule* PayloadReceiverTargetModule::getTeam() {
	return team;
}

ILivingEntityModule* PayloadReceiverTargetModule::getLivingEntity() {
	return livingEntity;
}

void PayloadReceiverTargetModule::createBMain() {
    super::createBMain();

    team = e->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
    livingEntity = e->getComponentAs<ILivingEntityModule*>(true/*mustExist*/);
}

PayloadReceiverTargetModule::~PayloadReceiverTargetModule() {
	//void
}
