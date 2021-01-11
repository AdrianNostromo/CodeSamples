#include "PBodyConfig.h"

using namespace rpg3D;

const int PBodyConfig::UssageBitGroups::None = 1;
const int PBodyConfig::UssageBitGroups::Movement = 2;
const int PBodyConfig::UssageBitGroups::CardinalRotation = 4;
const int PBodyConfig::UssageBitGroups::VisualsUpdate = 8;

PBodyConfig::PBodyConfig(
	int ussageBitMask, 
	DynamicsType dynamicsType,
	Vector3* physicsBodyOffsetPos, Quaternion* physicsBodyOffsetRot,
	physics3D::PhysicsShapeConfig* pShapeConfig,
	float mass, float friction, float rollingFriction,
	Vector3 enabledRotationAxes,
	float linearDamping, float angularDamping,
	Vector3* customGravity)
	: ussageBitMask(ussageBitMask), 
	dynamicsType(dynamicsType),
	physicsBodyOffsetPos(physicsBodyOffsetPos), physicsBodyOffsetRot(physicsBodyOffsetRot),
	pShapeConfig(pShapeConfig),
	mass(mass), friction(friction), rollingFriction(rollingFriction),
	enabledRotationAxes(enabledRotationAxes),
	linearDamping(linearDamping), angularDamping(angularDamping),
	customGravity(customGravity)
{
	//void
}
