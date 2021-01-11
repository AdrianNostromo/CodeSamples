#pragma once

#include <base/gh.h>
#include "P3DM_Base.h"
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>
#include <base/math/Quaternion.h>
#include <btBulletDynamicsCommon.h>
#include <base/list/Array2D.h>
#include <base/physics3D/config/ConfiguredPBody.h>

class VoxelGridDrawable3D;
class IContainer3D;
class btTypedConstraint;
namespace base {
	class GWEvent;
	class EntityEvent;
};
namespace graphics {
	class TilingInformation;
}
namespace physics3D {
	class PhysicsShapeConfig;
};
namespace rpg3D {
	class IPActorControllerBase;
	class IPBodyUpdateHandler;
	class PBodyConfig;
	class CallbackMotionStatePBodyUpdateHandler;
	class PhysicsConstraintConfig;

class P3DM_Body : public P3DM_Base {priv typedef P3DM_Base super;pub dCtor(P3DM_Body)
	priv class ConfiguredConstraint {pub dCtor(ConfiguredConstraint);
		pub rpg3D::PhysicsConstraintConfig* config;
		pub btTypedConstraint* pConstraint;

		pub bool isInPWorld = false;

		pub explicit ConfiguredConstraint(
			rpg3D::PhysicsConstraintConfig* config, btTypedConstraint* pConstraint)
			: config(config), pConstraint(pConstraint)
		{}
	};

	// Don't change these with the current system. The PhysicsActor also has a copy of these.
	prot int physicsGroupsBitMap;
	prot int anyInteraction_maskBitMap;
	prot int hardImpact_maskBitMap;

	priv std::shared_ptr<graphics::TilingInformation> tilingInformation = nullptr;

	prot ArrayList<physics3D::ConfiguredPBody> pBodiesConfiguredList{};
	prot physics3D::ConfiguredPBody* pConfiguredBodyMovement = nullptr;
	prot btRigidBody* pBodyMovement = nullptr;
	prot physics3D::ConfiguredPBody* pConfiguredBodyCardinalRotation = nullptr;
	prot btRigidBody* pBodyCardinalRotation = nullptr;
	prot physics3D::ConfiguredPBody* pConfiguredBodyVisualsAnchor = nullptr;
	prot btRigidBody* pBodyVisualsAnchor = nullptr;

	priv ArrayList<ConfiguredConstraint> pConfiguredConstraintsList{};

	IPActorControllerBase* pAction = nullptr;
	void* ptr_pAction = nullptr;

	priv float massMultiplier = 1.0f;
	// This will also toggle the object kinematic flag (btCollisionObject::CF_KINEMATIC_OBJECT).
	priv bool isKinematicallyFrozen = false;

	// This is used with the subclass positional handling logic.
	priv bool isPositionalSyncListenerEnabled = false;
	priv bool isPositionalSyncListenerInitialised = false;
	priv ArrayList<std::shared_ptr<IEventListener>> syncAutoListenersList{};

	prot Vector3 cachedPos{0.0f, 0.0f, 0.0f};
	prot Quaternion cachedRot{};
	prot Vector3 cachedScale{1.0f, 1.0f, 1.0f};

	prot btDiscreteDynamicsWorld* pWorld = nullptr;

	pub explicit P3DM_Body(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);
	prot void createMain() override;
	prot void createBMain() override;

	pub void setTilingInformation(std::shared_ptr<graphics::TilingInformation> tilingInformation) final;
	
	pub virtual void setMassMultiplier(float massMultiplier) final;
	pub virtual void setIsKinematicallyFrozen(bool isKinematicallyFrozen) final;
	
	priv float syncMass();
	priv float syncSinglePBodyMass(btRigidBody* pBody/*pBody*/, btCollisionShape* pShape, rpg3D::PBodyConfig* pBodyConfig, btVector3& localInertia);

	priv void onMotionStateTransformChanged();

	priv static btTypedConstraint* CreateConstraint(PhysicsConstraintConfig* pConstraintsConfig, ArrayList<physics3D::ConfiguredPBody>& pBodiesConfiguredList);

	prot void setIsPositionalSyncListenerEnabled(bool isPositionalSyncListenerEnabled);

	pub btRigidBody* getPBodyVisualsAnchor() override;
	pub btRigidBody* getPBodyMovement() override;
	prot btTransform getPBodyVisualTransform();

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	prot void syncPos(Vector3* pos);
	prot void syncPos(float x, float y, float z);
	prot void syncPosX(float x);
	prot void syncPosY(float y);
	prot void syncPosZ(float z);
	prot void syncRot(Quaternion& rot);
	prot void syncRot(Vector3& axis, float degrees);
	prot void syncRotEulerZYX(Vector3& axis, float degrees);
	prot void syncCardinalRotation(float degrees);
	prot void syncScale(Vector3* scale);

	pub void activateAllPBodies() final;

	prot void disposePre() override;
	prot void disposeMain() override;
    pub ~P3DM_Body() override;
};
};
