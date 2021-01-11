#pragma once

#include <base/gh.h>
#include "IPBodyUpdateHandler.h"

namespace rpg3D {
ATTRIBUTE_ALIGNED16(class) DirectPBodyUpdateHandler : virtual public IPBodyUpdateHandler{pub dCtor(DirectPBodyUpdateHandler);
	// Managed.
	prot btTransform* physicsOffsetTransform_managed;
	prot void* ptr_physicsOffsetTransform_managed;
	
	prot btTransform visualTransform;

	priv btRigidBody* pBody = nullptr;
    
	pub explicit DirectPBodyUpdateHandler(btTransform* physicsOffsetTransform_managed, void* ptr_physicsOffsetTransform_managed);
	
	pub btRigidBody* peekPBody() final;
	pub void initPBody(btRigidBody* pBody) final;

	pub btTransform& getVisualWorldTransform() final;
	pub void setVisualWorldTransform(const btTransform& transform) final;

	pub void setVisualPos(const btVector3& pos) final;
	pub void setVisualPosX(float x) final;
	pub void setVisualPosY(float y) final;
	pub void setVisualPosZ(float z) final;

	pub void setVisualRot(btQuaternion& rot) final;
	pub void setVisualRot(btVector3& axis, float angDeg) final;
	pub void setVisualRotOnSingleAxis(btVector3& axis, float degrees) final;

	priv inline void applyVisualTransformToPhysics();

    pub ~DirectPBodyUpdateHandler() override;
};
};
