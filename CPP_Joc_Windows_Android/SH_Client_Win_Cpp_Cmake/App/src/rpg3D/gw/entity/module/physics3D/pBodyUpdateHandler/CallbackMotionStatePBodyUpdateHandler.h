#pragma once

#include <base/gh.h>
#include "DirectPBodyUpdateHandler.h"
#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>
#include <functional>
#include "IPBodyUpdateHandler.h"

///The CallbackMotionStatePBodyUpdateHandler provides a common implementation to synchronize world transforms with offsets.
namespace rpg3D {
ATTRIBUTE_ALIGNED16(class) CallbackMotionStatePBodyUpdateHandler : public DirectPBodyUpdateHandler, virtual public btMotionState {priv typedef DirectPBodyUpdateHandler super;pub dCtor(CallbackMotionStatePBodyUpdateHandler);
	pub std::function<void()> cb_onTransformChanged;
	
	//BT_DECLARE_ALIGNED_ALLOCATOR();
	pub CallbackMotionStatePBodyUpdateHandler(btTransform* physicsOffsetTransform_managed, void* ptr_physicsOffsetTransform_managed, std::function<void()> cb_onTransformChanged);

	///synchronizes world transform from user to physics
	pub void getWorldTransform(btTransform& transform) const override;

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	pub void setWorldTransform(const btTransform& transform) override;

	pub ~CallbackMotionStatePBodyUpdateHandler() override;
};
};
