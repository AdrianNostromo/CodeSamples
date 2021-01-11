#pragma once

#include <base/gh.h>
#include "GPP3D_Contacts.h"
#include <base/runtimeSettings/IRuntimeSettings.h>

class IEventListener;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btBroadphaseInterface;
struct btDefaultCollisionConstructionInfo;
class btDefaultCollisionConfiguration;
class btCollisionShape;
class btRigidBody;

namespace base {
	class GWEvent;
};
namespace rpg3D {
	class CustomCollisionDispatcher;
};
namespace physics3D {
	class Physics3DRenderer;
}

namespace rpg3D {
class GPP3D_Bullet : public GPP3D_Contacts {priv typedef GPP3D_Contacts super;pub dCtor(GPP3D_Bullet);
	priv static GPP3D_Bullet* _THIS;

	priv btDiscreteDynamicsWorld* pWorld = nullptr;
	priv btSequentialImpulseConstraintSolver* solver = nullptr;
	priv btBroadphaseInterface* overlappingPairCache = nullptr;
	priv CustomCollisionDispatcher* dispatcher = nullptr;

	priv btDefaultCollisionConstructionInfo* constructionInfo = nullptr;
	priv void* ptr_m_persistentManifoldPool = nullptr;
	priv btDefaultCollisionConfiguration* collisionConfiguration = nullptr;

	priv btCollisionShape* groundPlaneCollisionShape = nullptr;
	priv btRigidBody* groundPlaneCollisionRigidBody = nullptr;

	priv bool isGroundPlaneActive = false;

	priv physics3D::Physics3DRenderer* physicsRenderer = nullptr;
	priv int activeRenderableId = -1;
	priv std::shared_ptr<IEventListener> debugSettingListener_physics3DDebugRender = nullptr;
	priv std::shared_ptr<IEventListener> debugSettingListener_physics3DDebugClearDepthBuffer = nullptr;

	pub explicit GPP3D_Bullet(base::IGameWorld* gw);
	prot void createBMain() override;
	priv void onGWEvent_createContent(IEventListener& eventListener, base::GWEvent& event);

	priv void onRuntimeSettings_settingChanged(IEventListener& eventListener, base::IRuntimeSettings::EventRuntimeSetting& event);

	priv static void _PhysicsTickCallback(btDynamicsWorld* pWorld, float timeStep);
	priv void physicsTickCallback(btDynamicsWorld* pWorld, float timeStep);

	pub btDiscreteDynamicsWorld* getDynamicsWorld() final;

	pub void setIsGroundPlaneActive(bool isGroundPlaneActive) final;

	priv void onGWEvent_gameLoop_physicsSimulation(IEventListener& eventListener, base::GWEvent& event);

	prot void disposeMain() override;
	pub ~GPP3D_Bullet() override;
};
};
