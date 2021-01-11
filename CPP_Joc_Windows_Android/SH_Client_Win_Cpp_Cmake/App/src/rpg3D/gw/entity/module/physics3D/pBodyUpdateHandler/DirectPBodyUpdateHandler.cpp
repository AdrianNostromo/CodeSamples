#include "DirectPBodyUpdateHandler.h"
#include <base/exceptions/LogicException.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <base/math/Math.h>

using namespace rpg3D;

DirectPBodyUpdateHandler::DirectPBodyUpdateHandler(btTransform* physicsOffsetTransform_managed, void* ptr_physicsOffsetTransform_managed)
	: physicsOffsetTransform_managed(physicsOffsetTransform_managed),
	ptr_physicsOffsetTransform_managed(ptr_physicsOffsetTransform_managed)
{
	visualTransform.setIdentity();
}

btRigidBody* DirectPBodyUpdateHandler::peekPBody() {
	return pBody;
}

void DirectPBodyUpdateHandler::initPBody(btRigidBody* pBody) {
	this->pBody = pBody;
}

btTransform& DirectPBodyUpdateHandler::getVisualWorldTransform() {
	return visualTransform;
}

void DirectPBodyUpdateHandler::setVisualWorldTransform(const btTransform& visualTransform) {
	this->visualTransform = visualTransform;

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualPos(const btVector3& pos) {
	visualTransform.setOrigin(pos);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualPosX(float x) {
	visualTransform.getOrigin().setX(x);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualPosY(float y) {
	visualTransform.getOrigin().setY(y);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualPosZ(float z) {
	visualTransform.getOrigin().setZ(z);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualRot(btQuaternion& rot) {
	visualTransform.setRotation(rot);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualRot(btVector3& axis, float angDeg) {
	btQuaternion q = visualTransform.getRotation();
	q.setRotation(axis, Math::toRadians(angDeg));
	visualTransform.setRotation(q);

	applyVisualTransformToPhysics();
}

void DirectPBodyUpdateHandler::setVisualRotOnSingleAxis(btVector3& axis, float degrees) {
	btQuaternion q = visualTransform.getRotation();
	
	float yawZ;
	float pitchY;
	float rollX;
	q.getEulerZYX(yawZ, pitchY, rollX);
	if (axis.x() == 1.0f) {
		rollX = Math::toRadians(degrees);
	}else if (axis.y() == 1.0f) {
		pitchY = Math::toRadians(degrees);
	}else if (axis.z() == 1.0f) {
		yawZ = Math::toRadians(degrees);
	}else {
		throw LogicException(LOC);
	}
	q.setEulerZYX(yawZ, pitchY, rollX);
	visualTransform.setRotation(q);

	applyVisualTransformToPhysics();
}

inline void DirectPBodyUpdateHandler::applyVisualTransformToPhysics() {
	if (this->physicsOffsetTransform_managed != nullptr) {
		btTransform tPos{};
		tPos.setIdentity();
		tPos.setOrigin(visualTransform.getOrigin());

		btTransform tRot{};
		tRot.setIdentity();
		tRot.setRotation(visualTransform.getRotation());

		tPos = tPos * *this->physicsOffsetTransform_managed;
		tPos = tPos * tRot;

		if (pBody != nullptr) {
			if (pBody->getActivationState() != ACTIVE_TAG) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				pBody->activate(true);
			}

			pBody->getWorldTransform() = tPos;
		}
	} else {
		if (pBody != nullptr) {
			if (pBody->getActivationState() != ACTIVE_TAG) {
				// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
				pBody->activate(true);
			}

			pBody->getWorldTransform() = this->visualTransform;
		}
	}
}

DirectPBodyUpdateHandler::~DirectPBodyUpdateHandler() {
	if (physicsOffsetTransform_managed != nullptr) {
		physicsOffsetTransform_managed->~btTransform();
		btAlignedFree(ptr_physicsOffsetTransform_managed);

		physicsOffsetTransform_managed = nullptr;
	}
}
