#pragma once

#include <base/gh.h>
#include <LinearMath/btTransform.h>

class btRigidBody;

namespace rpg3D {
class IPBodyUpdateHandler {
	pub virtual btRigidBody* peekPBody() = 0;
	pub virtual void initPBody(btRigidBody* pBody) = 0;

	pub virtual btTransform& getVisualWorldTransform() = 0;
	pub virtual void setVisualWorldTransform(const btTransform& transform) = 0;
	
	pub virtual void setVisualPos(const btVector3& pos) = 0;
	pub virtual void setVisualPosX(float x) = 0;
	pub virtual void setVisualPosY(float y) = 0;
	pub virtual void setVisualPosZ(float z) = 0;

	pub virtual void setVisualRot(btQuaternion& rot) = 0;
	pub virtual void setVisualRot(btVector3& axis, float angDeg) = 0;
	pub virtual void setVisualRotOnSingleAxis(btVector3& axis, float degrees) = 0;

    pub virtual ~IPBodyUpdateHandler() = default;
};
};
