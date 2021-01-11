#pragma once

#include <base/gh.h>
#include "P3DM_InterEntitiesConstraints.h"

namespace rpg3D {
class P3DM_Dynamics : public P3DM_InterEntitiesConstraints {priv typedef P3DM_InterEntitiesConstraints super;pub dCtor(P3DM_Dynamics);
	priv struct PersistentLinearForce {
		pub bool isActive = false;
		pub bool applyToMovementPBodyOnly = false;
		pub Vector3 force{};
	};
	priv struct PersistentAngularVelocity {
		pub bool isActive = false;
		pub Vector3 angularVelocity{};
	};

	priv PersistentLinearForce persistentLinearForce{};
	priv PersistentAngularVelocity persistentAngularVelocity{};

	// This is used because multiple logics can use the loop.
	priv int loopUsersCount = 0;
	priv ArrayList<std::shared_ptr<IEventListener>> loopListenersList{};

	pub explicit P3DM_Dynamics(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);
	prot void createMain() override;
	
	// Note. Call this only on change.
	priv void modifyLoopUsersCount(bool loopUserStateChange);

	pub void clearAllForces() final;

	pub void setLinearForce(
		float x, float y, float z, bool isContinuousUpdate, 
		float* linearDamping, bool applyToMovementPBodyOnly,
		bool useCentralForceInsteadOfVelocitySet) final;

	pub void setAngularVelocity(float x, float y, float z, bool isContinuousUpdate, float* angularDamping) final;

	pub void setDamping(float linearDamping, float angularDamping) final;
	pub void setLinearDamping(float linearDamping) final;
	pub void setAngularDamping(float angularDamping) final;

	pub void applyImpulse(float x, float y, float z) final;

	priv void onGWEvent_gameLoop_prePhysicsSimulation(IEventListener& eventListener, base::GWEvent& event);

    pub ~P3DM_Dynamics() override;
};
};
