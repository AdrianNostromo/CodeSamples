#include "CallbackMotionStatePBodyUpdateHandler.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

CallbackMotionStatePBodyUpdateHandler::CallbackMotionStatePBodyUpdateHandler(btTransform* physicsOffsetTransform_managed, void* ptr_physicsOffsetTransform_managed, std::function<void()> cb_onTransformChanged)
	: super(physicsOffsetTransform_managed, ptr_physicsOffsetTransform_managed),
	cb_onTransformChanged(cb_onTransformChanged)
{
	//void
}

///synchronizes world transform from user to physics
void CallbackMotionStatePBodyUpdateHandler::getWorldTransform(btTransform& transform) const {
	transform = this->physicsOffsetTransform_managed != nullptr ? this->visualTransform * *this->physicsOffsetTransform_managed : this->visualTransform;
}

///synchronizes world transform from physics to user
///Bullet only calls the update of worldtransform for active objects
void CallbackMotionStatePBodyUpdateHandler::setWorldTransform(const btTransform& transform) {
	this->visualTransform = this->physicsOffsetTransform_managed != nullptr ? transform * this->physicsOffsetTransform_managed->inverse() : transform;
	
	if (cb_onTransformChanged) {
		cb_onTransformChanged();
	}
}

CallbackMotionStatePBodyUpdateHandler::~CallbackMotionStatePBodyUpdateHandler() {
	//void
}
