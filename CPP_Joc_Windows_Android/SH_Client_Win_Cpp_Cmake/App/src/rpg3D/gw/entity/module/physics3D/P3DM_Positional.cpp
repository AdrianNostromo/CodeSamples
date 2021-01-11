#include "P3DM_Positional.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/exceptions/LogicException.h>
#include <btBulletDynamicsCommon.h>
#include <base/math/Math.h>

using namespace rpg3D;

const int P3DM_Positional::POSITIONAL_HEANDLER_PRIORITY = 2;

P3DM_Positional::P3DM_Positional(
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

void P3DM_Positional::createMain() {
	super::createMain();

	e->registerPositional3DHandler(this, POSITIONAL_HEANDLER_PRIORITY);

	setIsPositionalSyncListenerEnabled(!this->isMainPositionalHandler);
}

void P3DM_Positional::setMainIsPositionalHandler(bool isMainPositionalHandler) {
	if (this->isMainPositionalHandler == isMainPositionalHandler) {
		throw LogicException(LOC);
	}

	this->isMainPositionalHandler = isMainPositionalHandler;

	setIsPositionalSyncListenerEnabled(!this->isMainPositionalHandler);
}

Vector3* P3DM_Positional::getPos() {
	btVector3& v = getPBodyVisualTransform().getOrigin();

	t_v3.set(v.getX(), v.getY(), v.getZ());

	return &t_v3;
}

float P3DM_Positional::getX() {
	return getPBodyVisualTransform().getOrigin().getX();
}

float P3DM_Positional::getY() {
	return getPBodyVisualTransform().getOrigin().getY();
}

float P3DM_Positional::getZ() {
	return getPBodyVisualTransform().getOrigin().getZ();
}

void P3DM_Positional::setPos(float x, float y, float z) {
	btVector3& v = getPBodyVisualTransform().getOrigin();
	if (v.x() == x && v.y() == y && v.z() == z) {
		return;
	}

	syncPos(x, y, z);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void P3DM_Positional::setPos(Vector3& pos) {
	btVector3& v = getPBodyVisualTransform().getOrigin();
	if (v.x() == pos.x && v.y() == pos.y && v.z() == pos.z) {
		return;
	}

	syncPos(&pos);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void P3DM_Positional::setX(float x) {
	btVector3& v = getPBodyVisualTransform().getOrigin();
	if (v.x() == x) {
		return;
	}

	syncPosX(x);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void P3DM_Positional::setY(float y) {
	btVector3& v = getPBodyVisualTransform().getOrigin();
	if (v.y() == y) {
		return;
	}

	syncPosY(y);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

void P3DM_Positional::setZ(float z) {
	btVector3& v = getPBodyVisualTransform().getOrigin();
	if (v.z() == z) {
		return;
	}

	syncPosZ(z);

	e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	e->dispatchEvent(*IWorldEntity::EVENT_posManuallyChanged);
}

Quaternion* P3DM_Positional::getRot() {
	/*btQuaternion q1{ btVector3{1.0f, 0.0f, 0.0f}, Math::toRadians(-88.0f) };
	float yawZ1;
	float pitchY1;
	float rollX1;
	q1.getEulerZYX(yawZ1, pitchY1, rollX1);

	btQuaternion q2{ btVector3{0.0f, 1.0f, 0.0f}, Math::toRadians(-88.0f) };
	float yawZ2;
	float pitchY2;
	float rollX2;
	q2.getEulerZYX(yawZ2, pitchY2, rollX2);

	btQuaternion q3{ btVector3{0.0f, 0.0f, 1.0f}, Math::toRadians(-88.0f) };
	float yawZ3;
	float pitchY3;
	float rollX3;
	q3.getEulerZYX(yawZ3, pitchY3, rollX3);*/

	btQuaternion q = getPBodyVisualTransform().getRotation();
	t_q.set(q.x(), q.y(), q.z(), q.w());
	
	return &t_q;
}

void P3DM_Positional::setRot(Quaternion& rot) {
	btQuaternion q = getPBodyVisualTransform().getRotation();
	if (q.x() == rot.x && q.y() == rot.y && q.z() == rot.z && q.w() == rot.w) {
		return;
	}

	syncRot(rot);

	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void P3DM_Positional::setRot(Vector3& axis, float degrees) {
	btQuaternion q = getPBodyVisualTransform().getRotation();
	Quaternion rot{ axis, degrees };
	if (q.x() == rot.x && q.y() == rot.y && q.z() == rot.z && q.w() == rot.w) {
		return;
	}

	syncRot(axis, degrees);
	
	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void P3DM_Positional::setRotEulerZYX(Vector3& axis, float degrees) {
	btQuaternion q = getPBodyVisualTransform().getRotation();
	float yawZ;
	float pitchY;
	float rollX;
	q.getEulerZYX(yawZ, pitchY, rollX);
	if (axis.x == 1.0f) {
		if (rollX == Math::toRadians(degrees)) {
			return;
		}
	} else if (axis.y == 1.0f) {
		if (pitchY == Math::toRadians(degrees)) {
			return;
		}
	} else if (axis.z == 1.0f) {
		if (yawZ == Math::toRadians(degrees)) {
			return;
		}
	} else {
		throw LogicException(LOC);
	}

	syncRotEulerZYX(axis, degrees);
	
	e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	e->dispatchEvent(*IWorldEntity::EVENT_rotManuallyChanged);
}

void P3DM_Positional::setCardinalRotation(float degrees) {
	syncCardinalRotation(degrees);

	e->dispatchEvent(*IWorldEntity::EVENT_syncCardinalRotation);
}

Vector3* P3DM_Positional::getScale() {
	return &cachedScale;
}

void P3DM_Positional::setScale(float scale) {
	Vector3 v(scale, scale, scale);
	syncScale(&v);

	e->dispatchEvent(*IWorldEntity::EVENT_syncScale);
}

P3DM_Positional::~P3DM_Positional() {
	//void
}
