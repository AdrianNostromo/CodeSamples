#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include "util/IPhysicsEntityCallbackFilter.h"
#include <functional>
#include <memory>

class btRigidBody;

namespace graphics {
	class TilingInformation;
};
namespace rpg3D {
	class IPhysicsEntityCallbackFilter;
	class CfgConstraint3D;
	class IConstraint3D;
};

namespace rpg3D {
class IPhysics3DModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual void setTilingInformation(std::shared_ptr<graphics::TilingInformation> tilingInformation) = 0;

	// This is used after teleports probably.
	pub virtual void clearAllForces() = 0;

	pub virtual void setLinearForce(
		float x, float y, float z, bool isContinuousUpdate,
		float* linearDamping, bool applyToMovementPBodyOnly,
		bool useCentralForceInsteadOfVelocitySet) = 0;

	pub virtual void setAngularVelocity(float x, float y, float z, bool isContinuousUpdate, float* angularDamping) = 0;

	pub virtual void setDamping(float linearDamping, float angularDamping) = 0;
	pub virtual void setLinearDamping(float linearDamping) = 0;
	pub virtual void setAngularDamping(float angularDamping) = 0;

	pub virtual void applyImpulse(float x, float y, float z) = 0;

	pub virtual void setMassMultiplier(float massMultiplier) = 0;
	// This has no effect if the object has 0 mass (is kinematic or static already).
	pub virtual void setIsKinematicallyFrozen(bool isKinematicallyFrozen) = 0;

	pub virtual IPhysicsEntityCallbackFilter* addEntityContactFilter(std::function<void(
		IPhysicsEntityCallbackFilter* filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact) = 0;
	pub virtual void removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) = 0;

	pub virtual btRigidBody* getPBodyVisualsAnchor() = 0;
	pub virtual btRigidBody* getPBodyMovement() = 0;

	pub virtual void activateAllPBodies() = 0;

	pub virtual IConstraint3D* createConstraint(std::shared_ptr<CfgConstraint3D> config, IPhysics3DModule* target) = 0;
	pub virtual void pushRemoteConstraint(IConstraint3D* remoteConstraint) = 0;
	pub virtual void removeRemoteConstraint(IConstraint3D* remoteConstraint) = 0;

    pub ~IPhysics3DModule() override = default;
};
};
