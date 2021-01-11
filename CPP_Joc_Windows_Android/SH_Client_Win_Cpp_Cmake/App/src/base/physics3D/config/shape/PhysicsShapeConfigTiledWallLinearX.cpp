#include "PhysicsShapeConfigTiledWallLinearX.h"
#include <cassert>

using namespace physics3D;

int PhysicsShapeConfigTiledWallLinearX::TYPE = super::NewType();

PhysicsShapeConfigTiledWallLinearX::PhysicsShapeConfigTiledWallLinearX(
	float collisionMargin,
	Vector3 tileSize)
	: super(TYPE),
	collisionMargin(collisionMargin),
	tileSize(std::move(tileSize))
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigTiledWallLinearX::~PhysicsShapeConfigTiledWallLinearX() {
	//void
}
