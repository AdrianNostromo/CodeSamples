#include "P3DM_InterEntitiesConstraints.h"
#include "constraint3D/Constraint3D.h"
#include "constraint3D/config/CfgConstraint3D6Dof.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

P3DM_InterEntitiesConstraints::P3DM_InterEntitiesConstraints(
	IWorldEntity* entity,
	Physics3DTemplate* t,
	int physicsGroupsBitMap,
	int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	:super(entity, t,
		physicsGroupsBitMap,
		anyInteraction_maskBitMap, hardImpact_maskBitMap)
{
	//void
}

void P3DM_InterEntitiesConstraints::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	for (int i = 0; i < constraintsList.count(); i++) {
		std::shared_ptr<Constraint3D> cs = constraintsList.getDirect(i);

		cs->addToPWorld(pWorld);
	}
}

void P3DM_InterEntitiesConstraints::disconnectFromGamePartsPre() {
	for (int i = 0; i < constraintsList.count(); i++) {
		std::shared_ptr<Constraint3D> cs = constraintsList.getDirect(i);

		cs->removeFromPWorld();
	}

	super::disconnectFromGamePartsPre();
}

IConstraint3D* P3DM_InterEntitiesConstraints::createConstraint(std::shared_ptr<CfgConstraint3D> config, IPhysics3DModule* target) {
	std::shared_ptr<Constraint3D> constraint = buildConstraint(config, target);

	constraintsList.appendReference(constraint);

	target->pushRemoteConstraint(constraint.get());

	if (!pBodyVisualsAnchor->isActive()) {
		// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
		pBodyVisualsAnchor->activate(true);
	}
	if (!target->getPBodyMovement()->isActive()) {
		// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
		target->getPBodyMovement()->activate(true);
	}

	if (e->getIsInGameWorldAndActive()) {
		constraint->addToPWorld(pWorld);
	}

	return constraint.get();
}

void P3DM_InterEntitiesConstraints::pushRemoteConstraint(IConstraint3D* remoteConstraint) {
	if (remoteConstraintsList.containsDirect(remoteConstraint)) {
		throw LogicException(LOC);
	}

	remoteConstraintsList.appendDirect(remoteConstraint);
}

void P3DM_InterEntitiesConstraints::removeRemoteConstraint(IConstraint3D* remoteConstraint) {
	if (!remoteConstraintsList.removeDirect(remoteConstraint)) {
		throw LogicException(LOC);
	}
}

void P3DM_InterEntitiesConstraints::removeConstraint(Constraint3D* constraint) {
	for (int i = 0; i < constraintsList.count(); i++) {
		std::shared_ptr<Constraint3D> cs = constraintsList.getDirect(i);

		if (cs.get() == constraint) {
			if (!pBodyVisualsAnchor->isActive()) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				pBodyVisualsAnchor->activate(true);
			}
			if (!cs->otherTarget->getPBodyMovement()->isActive()) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				cs->otherTarget->getPBodyMovement()->activate(true);
			}

			cs->removeFromPWorld();

			cs->otherTarget->removeRemoteConstraint(cs.get());

			constraintsList.remove(i);

			return;
		}
	}

	throw LogicException(LOC);
}

std::shared_ptr<Constraint3D> P3DM_InterEntitiesConstraints::buildConstraint(std::shared_ptr<CfgConstraint3D> config, IPhysics3DModule* target) {
	if (config->type == CfgConstraint3D6Dof::TYPE) {
		CfgConstraint3D6Dof* cConfig = config->getAs<CfgConstraint3D6Dof*>(CfgConstraint3D6Dof::TYPE);

		/*btGeneric6DofConstraint* cs = new btGeneric6DofConstraint(
			*getPBodyVisualsAnchor(), *target->getPBodyMovement(),
			cConfig->offsetInActorA, cConfig->offsetInActorB,
			cConfig->useLinearReferenceFrameA
		);*/

		btPoint2PointConstraint* cs = new btPoint2PointConstraint(
			*getPBodyVisualsAnchor(), *target->getPBodyMovement(),
			cConfig->offsetInActorA.getOrigin(), cConfig->offsetInActorB.getOrigin()
		);

		cs->m_setting.m_impulseClamp = 2.0f;
		cs->m_setting.m_tau = 0.001f;

		// Lock pos axes.
		//cs->setLimit(0, 0, 0);
		//cs->setLimit(1, 0, 0);
		//cs->setLimit(2, 0, 0);

		//// Lock rot axes.
		//cs->setLimit(3, 0, 0);
		//cs->setLimit(4, 0, 0);
		//cs->setLimit(5, 0, 0);

		std::shared_ptr<Constraint3D> constraint{new Constraint3D(
			this/*handler*/, config,
			cs,
			target
		)};

		return constraint;
	} else {
		// Unahndler constraint type.
		throw LogicException(LOC);
	}
}

void P3DM_InterEntitiesConstraints::disposeMain() {
	if (constraintsList.count() != 0 || remoteConstraintsList.count() != 0) {
		// The constraint handlers must breake the constraint in their disposePre.S
		throw LogicException(LOC);
	}

	super::disposeMain();
}

P3DM_InterEntitiesConstraints::~P3DM_InterEntitiesConstraints() {
	//void
}
