#include "PhysicsShapeConfig.h"

using namespace physics3D;

int PhysicsShapeConfig::NewType() {
	static int COUNTER = 0;

	COUNTER++;

	return COUNTER;
}

PhysicsShapeConfig::PhysicsShapeConfig(int type)
	: type(type)
{
	//void
}

PhysicsShapeConfig::~PhysicsShapeConfig() {
	//void
}
