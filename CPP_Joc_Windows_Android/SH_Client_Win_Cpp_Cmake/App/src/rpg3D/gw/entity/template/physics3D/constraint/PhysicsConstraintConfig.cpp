#include "PhysicsConstraintConfig.h"

using namespace rpg3D;

int PhysicsConstraintConfig::NewType() {
	static int COUNTER = 0;

	COUNTER++;

	return COUNTER;
}

PhysicsConstraintConfig::PhysicsConstraintConfig(int type, bool disableCollisionsBetweenLinkedBodies)
	: type(type), disableCollisionsBetweenLinkedBodies(disableCollisionsBetweenLinkedBodies)
{
	//void
}

PhysicsConstraintConfig::~PhysicsConstraintConfig() {
	//void
}
