#include "PhysicsShapeConfigTiledWallGridXY.h"
#include <cassert>

using namespace physics3D;

int PhysicsShapeConfigTiledWallGridXY::TYPE = super::NewType();

PhysicsShapeConfigTiledWallGridXY::PhysicsShapeConfigTiledWallGridXY(
	float collisionMargin,
	Vector3 tileSize)
	: super(TYPE),
	collisionMargin(collisionMargin),
	tileSize(std::move(tileSize))
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigTiledWallGridXY::~PhysicsShapeConfigTiledWallGridXY() {
	//void
}
