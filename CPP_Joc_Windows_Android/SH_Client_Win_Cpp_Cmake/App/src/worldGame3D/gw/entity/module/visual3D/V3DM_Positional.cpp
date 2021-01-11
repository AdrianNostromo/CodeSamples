#include "V3DM_Positional.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/exceptions/LogicException.h>

using namespace worldGame3D;

const int V3DM_Positional::POSITIONAL_HEANDLER_PRIORITY = 1;

V3DM_Positional::V3DM_Positional(
	IWorldEntity* entity,
	Visual3DTemplate* t,
	ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable
)
	:super(
		entity,
		t,
		drawablesList, drawablesMap, rootDrawable)
{
	//void
}

void V3DM_Positional::createMain() {
	super::createMain();

	e->registerPositional3DHandler(this, POSITIONAL_HEANDLER_PRIORITY);
	
	setIsPositionalSyncListenerEnabled(!this->isMainPositionalHandler);
}

void V3DM_Positional::setMainIsPositionalHandler(bool isMainPositionalHandler) {
	if (this->isMainPositionalHandler == isMainPositionalHandler) {
		throw LogicException(LOC);
	}

	this->isMainPositionalHandler = isMainPositionalHandler;

	setIsPositionalSyncListenerEnabled(!this->isMainPositionalHandler);
}

Vector3* V3DM_Positional::getPos() {
	return rootDrawable->getPos();
}

float V3DM_Positional::getX() {
	return rootDrawable->getX();
}

float V3DM_Positional::getY() {
	return rootDrawable->getY();
}

float V3DM_Positional::getZ() {
	return rootDrawable->getZ();
}

void V3DM_Positional::setPos(float x, float y, float z) {
	if (rootDrawable->getPos()->equals(x, y, z)) {
		return;
	}

	syncPos(x, y, z);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void V3DM_Positional::setPos(Vector3& pos) {
	if (rootDrawable->getPos()->equals(pos)) {
		return;
	}

	syncPos(&pos);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void V3DM_Positional::setX(float x) {
	if (rootDrawable->getPos()->x == x) {
		return;
	}

	syncPosX(x);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void V3DM_Positional::setY(float y) {
	if (rootDrawable->getPos()->y == y) {
		return;
	}

	syncPosY(y);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void V3DM_Positional::setZ(float z) {
	if (rootDrawable->getPos()->z == z) {
		return;
	}

	syncPosZ(z);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

Quaternion* V3DM_Positional::getRot() {
	return rootDrawable->getRotation();
}

void V3DM_Positional::setRot(Quaternion& rot) {
	if (rootDrawable->getRotation()->equals(rot)) {
		return;
	}

	syncRot(rot);

	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void V3DM_Positional::setRot(Vector3& axis, float degrees) {
	Quaternion rot{ axis, degrees };
	if (rootDrawable->getRotation()->equals(rot)) {
		return;
	}

	syncRot(axis, degrees);
	
	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void V3DM_Positional::setRotEulerZYX(Vector3& axis, float degrees) {
	float yawZ;
	float pitchY;
	float rollX;
	rootDrawable->getRotation()->getEulerAnglesZYXDeg(yawZ, pitchY, rollX);

	if (axis.x == 1.0f) {
		if (rollX == degrees) {
			return;
		}
	} else if (axis.y == 1.0f) {
		if (pitchY == degrees) {
			return;
		}
	} else if (axis.z == 1.0f) {
		if (yawZ == degrees) {
			return;
		}
	} else {
		throw LogicException(LOC);
	}

	syncRotEulerZYX(axis, degrees);
	
	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void V3DM_Positional::setCardinalRotation(float degrees) {
	syncCardinalRotation(degrees);

	e->dispatchEvent(*IWorldEntity::EVENT_syncCardinalRotation);
}

Vector3* V3DM_Positional::getScale() {
	return rootDrawable->getScale();
}

void V3DM_Positional::setScale(float scale) {
	Vector3 v(scale, scale, scale);
	syncScale(&v);

	e->dispatchEvent(*IWorldEntity::EVENT_syncScale);
}

V3DM_Positional::~V3DM_Positional() {
	//void
}
