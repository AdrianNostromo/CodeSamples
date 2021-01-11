#include "CCCTM_Base.h"
#include "../../template/customCubesCannonTurret/CustomCubesCannonTurretTemplate.h"
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>

using namespace towerDefense_a_v1;

int CCCTM_Base::ActionGroup_AimToTarget = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

CCCTM_Base::CCCTM_Base(
	IWorldEntity* entity,
	CustomCubesCannonTurretTemplate* t,
	base::audio::ISound* activationSound)
	: super(entity),
	t(t),
	activationSound(activationSound)
{
	//void
}

void CCCTM_Base::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	visual3D = e->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

	groundBase = visual3D->getDrawableMustExist(t->visualId_groundPad);
	turret = visual3D->getDrawableMustExist(t->visualId_turret);
	barrel = visual3D->getDrawableMustExist(t->visualId_barrel);
	projectile = visual3D->getDrawableMustExist(t->visualId_projectile);

	barrel->setZ(t->barrelPosZ_normal);
}

void CCCTM_Base::setProjectileTemplateId(std::string& projectileTemplate_namespace, std::string& projectileTemplate_id) {
	this->projectileTemplate_namespace = projectileTemplate_namespace;
	this->projectileTemplate_id = projectileTemplate_id;
}

void CCCTM_Base::addProjectilePayload(std::shared_ptr<rpg3D::EffectPayload> payload) {
	projectilePayloadsList.appendReference(payload);
}

CCCTM_Base::~CCCTM_Base() {
	//void
}
