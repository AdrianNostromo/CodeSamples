#include "PhysicsConstraintConfigGeneric6Dof.h"

using namespace rpg3D;

int PhysicsConstraintConfigGeneric6Dof::TYPE = super::NewType();

PhysicsConstraintConfigGeneric6Dof::PhysicsConstraintConfigGeneric6Dof(
	int pBodyIndexA, int pBodyIndexB,
	Vector3 framePosInA, Vector3 framePosInB,
	Limit limitPosX, Limit limitPosY, Limit limitPosZ,
	Limit limitAngX, Limit limitAngY, Limit limitAngZ,
	bool disableCollisionsBetweenLinkedBodies)
	: super(TYPE, disableCollisionsBetweenLinkedBodies),
	pBodyIndexA(pBodyIndexA), pBodyIndexB(pBodyIndexB),
	framePosInA(framePosInA), framePosInB(framePosInB),
	limitPosX(limitPosX), limitPosY(limitPosY), limitPosZ(limitPosZ),
	limitAngX(limitAngX), limitAngY(limitAngY), limitAngZ(limitAngZ)
{
	rawSubclassPointer = this;
}

PhysicsConstraintConfigGeneric6Dof::~PhysicsConstraintConfigGeneric6Dof() {
	//void
}
