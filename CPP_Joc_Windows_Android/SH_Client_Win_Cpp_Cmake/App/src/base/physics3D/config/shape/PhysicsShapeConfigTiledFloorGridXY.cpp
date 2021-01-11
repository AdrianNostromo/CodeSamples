#include "PhysicsShapeConfigTiledFloorGridXY.h"
#include <cassert>

using namespace physics3D;

int PhysicsShapeConfigTiledFloorGridXY::TYPE = super::NewType();

PhysicsShapeConfigTiledFloorGridXY::PhysicsShapeConfigTiledFloorGridXY(
	float collisionMargin,
	Vector2 tileSize)
	: super(TYPE),
	collisionMargin(collisionMargin),
	tileSize(std::move(tileSize))
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigTiledFloorGridXY::~PhysicsShapeConfigTiledFloorGridXY() {
	//void
}
