#include "GPP3D_Bullet.h"
#include <btBulletDynamicsCommon.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/exceptions/LogicException.h>
#include "dispatcher/CustomCollisionDispatcher.h"
#include "manifold/CustomPersistentManifold.h"
#include <LinearMath/btPoolAllocator.h>
#include <rpg3D/gw/physics3D/util/WrappedPhysicsActor.h>
#include <base/physics3D/renderer/Physics3DRenderer.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <base/app/config/AppConfig.h>
#include <base/app/IApp.h>
#include <functional>
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <TestLog.h>

using namespace rpg3D;

GPP3D_Bullet* GPP3D_Bullet::_THIS = nullptr;

GPP3D_Bullet::GPP3D_Bullet(base::IGameWorld* gw)
	: super(gw)
{
	_THIS = this;

	gw->addEventListener(
		base::IGameWorld::GWEvent_createContent->type,
		std::bind(&GPP3D_Bullet::onGWEvent_createContent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_physicsSimulation->type,
		std::bind(&GPP3D_Bullet::onGWEvent_gameLoop_physicsSimulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPP3D_Bullet::createBMain() {
	super::createBMain();

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	constructionInfo = new btDefaultCollisionConstructionInfo();
	void* mem = btAlignedAlloc(sizeof(btPoolAllocator), 16);
	btPoolAllocator* poolAllocator;
	ptr_m_persistentManifoldPool = poolAllocator = new (mem) btPoolAllocator(sizeof(CustomPersistentManifold), constructionInfo->m_defaultMaxPersistentManifoldPoolSize);
	constructionInfo->m_persistentManifoldPool = poolAllocator;

	collisionConfiguration = new btDefaultCollisionConfiguration(*constructionInfo);

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = newt CustomCollisionDispatcher(collisionConfiguration);
	
	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	
	// The broadphase collision detection provides acceleration structure to quickly reject pairs of objects
	// based on axis aligned bounding box(AABB) overlap.Several different broadphase acceleration
	// structures are available:
	// btDbvtBroadphase uses a fast dynamic bounding volume hierarchy based on AABB tree
	// btAxisSweep3and bt32BitAxisSweep3 implement incremental 3d sweep and prune
	// btCudaBroadphase implements a fast uniform grid using GPU graphics hardware
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	pWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	pWorld->setGravity(btVector3(0.0f, 0.0f, -9.8f));
	pWorld->setInternalTickCallback(_PhysicsTickCallback);

	// This disables the motion_states interpolation between frames. Using this seems not optimal.
	// This causes massive slowdowns in the player movements.
	// NOTE: KEEP OFF.
	pWorld->setLatencyMotionStateInterpolation(false);
}

void GPP3D_Bullet::onGWEvent_createContent(IEventListener& eventListener, base::GWEvent& event) {
	// Create physics debug renderer.
	if (AppConfig::IS_PHYSICS_DEBUGGER_ENABLED) {
		if (gw->getMain()->getCC() == nullptr) {
			throw LogicException(LOC);
		}

		physicsRenderer = newt physics3D::Physics3DRenderer(gw->getApp(), pWorld, gw->getMain()->getCC());
		physicsRenderer->shapeDrawer.setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
		physicsRenderer->create();

		bool isDebugRenderSetting = gw->getApp()->getRuntimeSettings()->getSettingBool(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender, false);
		if (isDebugRenderSetting) {
			activeRenderableId = gw->getHandler()->getRenderingHandler()->addRenderable(
				nullptr/*stage*/, nullptr/*environment*/, physicsRenderer/*renderer*/, nullptr/*complexRenderer*/,
				IRenderingHandler::RenderOrderGroup::ROG_3D_DEBUG, IRenderingHandler::RendererProfilerType::RPT_SPECIAL
			);

			bool isDebugClearDepthBufferSetting = gw->getApp()->getRuntimeSettings()->getSettingBool(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugClearDepthBuffer, false);
			physicsRenderer->setIsClearDepthBuffer(isDebugClearDepthBufferSetting);
		}

		debugSettingListener_physics3DDebugRender = gw->getApp()->getRuntimeSettings()->addListener(
			rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender,
			std::bind(&GPP3D_Bullet::onRuntimeSettings_settingChanged, this, std::placeholders::_1, std::placeholders::_2)
		);
		debugSettingListener_physics3DDebugClearDepthBuffer = gw->getApp()->getRuntimeSettings()->addListener(
			rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugClearDepthBuffer,
			std::bind(&GPP3D_Bullet::onRuntimeSettings_settingChanged, this, std::placeholders::_1, std::placeholders::_2)
		);
	}
}

void GPP3D_Bullet::onRuntimeSettings_settingChanged(IEventListener& eventListener, base::IRuntimeSettings::EventRuntimeSetting& event) {
	if (&eventListener == debugSettingListener_physics3DDebugRender.get()) {
		if (!event.newWValue->getDirectAs_bool()) {
			if (activeRenderableId >= 0) {
				gw->getHandler()->getRenderingHandler()->removeRenderableMustExist(activeRenderableId);

				activeRenderableId = -1;
			}
		} else {
			if (activeRenderableId < 0) {
				activeRenderableId = gw->getHandler()->getRenderingHandler()->addRenderable(
					nullptr/*stage*/, nullptr/*environment*/, physicsRenderer/*renderer*/, nullptr/*complexRenderer*/,
					IRenderingHandler::RenderOrderGroup::ROG_3D_DEBUG, IRenderingHandler::RendererProfilerType::RPT_SPECIAL
				);

				bool isDebugClearDepthBufferSetting = gw->getApp()->getRuntimeSettings()->getSettingBool(rpg3D::IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugClearDepthBuffer, false);
				physicsRenderer->setIsClearDepthBuffer(isDebugClearDepthBufferSetting);
			}
		}
	} else if (&eventListener == debugSettingListener_physics3DDebugClearDepthBuffer.get()) {
		if (activeRenderableId >= 0) {
			physicsRenderer->setIsClearDepthBuffer(event.newWValue->getDirectAs_bool());
		}
	} else {
		throw LogicException(LOC);
	}
}

void GPP3D_Bullet::_PhysicsTickCallback(btDynamicsWorld* pWorld, float timeStep) {
	_THIS->physicsTickCallback(pWorld, timeStep);
}

void GPP3D_Bullet::physicsTickCallback(btDynamicsWorld* pWorld, float timeStep) {
	if (dispatcher->getTotalFullContactsCount() < 0 || dispatcher->getTotalFullContactsCount() > 1000) {
		// Handle missed contacts errors.
		throw LogicException(LOC);
	}

	ListDL<CustomCollisionDispatcher::ChangeEntry>::Entry* listEntry;
	while ((listEntry = dispatcher->changesList.getFirst()) != nullptr) {
		CustomCollisionDispatcher::ChangeEntry* changeInfo = &listEntry->data;

		if (changeInfo->wPActorA == nullptr || changeInfo->wPActorB == nullptr) {
			// Locig error, pActors should be valid.
			throw LogicException(LOC);
		}
		
		if (changeInfo->wPActorA->physicsActor == nullptr || changeInfo->wPActorB->physicsActor == nullptr) {
			// This may occur if entities are removed while this enumeration is in progress. But the entities are removed on a later delayed loop so this should never occur.
			throw LogicException(LOC);
		}

		changeInfo->wPActorA->physicsActor->getUsedInChangeContactsCount()--;
		changeInfo->wPActorB->physicsActor->getUsedInChangeContactsCount()--;
		if (changeInfo->wPActorA->physicsActor->getUsedInChangeContactsCount() < 0 || changeInfo->wPActorB->physicsActor->getUsedInChangeContactsCount() < 0) {
			// Logic error.
			throw LogicException(LOC);
		}

		processContactChange(changeInfo->wPActorA->physicsActor, changeInfo->wPActorB->physicsActor, changeInfo->isContact);
		
		listEntry->remove();
	}
}

btDiscreteDynamicsWorld* GPP3D_Bullet::getDynamicsWorld() {
	return pWorld;
}

void GPP3D_Bullet::setIsGroundPlaneActive(bool isGroundPlaneActive) {
	if (this->isGroundPlaneActive == isGroundPlaneActive) {
		return;
	}
	this->isGroundPlaneActive = isGroundPlaneActive;

	if (!this->isGroundPlaneActive) {
		if (groundPlaneCollisionRigidBody != nullptr) {
			pWorld->removeRigidBody(groundPlaneCollisionRigidBody);

			if (groundPlaneCollisionRigidBody->getMotionState()) {
				delete groundPlaneCollisionRigidBody->getMotionState();
			}

			delete groundPlaneCollisionRigidBody;
			groundPlaneCollisionRigidBody = nullptr;
		}

		if (groundPlaneCollisionShape != nullptr) {
			delete groundPlaneCollisionShape;
			groundPlaneCollisionShape = nullptr;
		}
	} else {
		if (groundPlaneCollisionShape != nullptr) {
			throw LogicException(LOC);
		}

		groundPlaneCollisionShape = new btBoxShape(btVector3(btScalar(10.0f), btScalar(10.0f), btScalar(10.0f)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, 0, -10));

		btScalar mass(0.0f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			groundPlaneCollisionShape->calculateLocalInertia(mass, localInertia);
		}

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundPlaneCollisionShape, localInertia);
		groundPlaneCollisionRigidBody = new btRigidBody(rbInfo);
		
		//add the body to the dynamics world
		pWorld->addRigidBody(groundPlaneCollisionRigidBody);
	}
}

void GPP3D_Bullet::onGWEvent_gameLoop_physicsSimulation(IEventListener& eventListener, base::GWEvent& event) {
	if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::PhysicsSimStart, std::chrono::system_clock::now()); }

	pWorld->stepSimulation(event.deltaS, 10, 1.0f / 120.0f);
	// 10, 1.0f / 120.0f @ 22-30 ms;
	// 10, 1.0f / 120.0f @ 22 ms;
	// 1, 1.0f / 60.0f @ 4-6 ms;

	if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::PhysicsSimEnd, std::chrono::system_clock::now()); }
}

void GPP3D_Bullet::disposeMain() {
	if (debugSettingListener_physics3DDebugRender != nullptr) {
		debugSettingListener_physics3DDebugRender->disconnect();
		debugSettingListener_physics3DDebugRender = nullptr;
	}
	if (debugSettingListener_physics3DDebugClearDepthBuffer != nullptr) {
		debugSettingListener_physics3DDebugClearDepthBuffer->disconnect();
		debugSettingListener_physics3DDebugClearDepthBuffer = nullptr;
	}
	if (physicsRenderer != nullptr) {
		if (activeRenderableId >= 0) {
			gw->getHandler()->getRenderingHandler()->removeRenderableMustExist(activeRenderableId);

			activeRenderableId = -1;
		}

		delete physicsRenderer;
		physicsRenderer = nullptr;
	}

	if (groundPlaneCollisionRigidBody != nullptr) {
		pWorld->removeRigidBody(groundPlaneCollisionRigidBody);

		if (groundPlaneCollisionRigidBody->getMotionState()) {
			delete groundPlaneCollisionRigidBody->getMotionState();
		}

		delete groundPlaneCollisionRigidBody;
		groundPlaneCollisionRigidBody = nullptr;
	}

	if (groundPlaneCollisionShape != nullptr) {
		delete groundPlaneCollisionShape;
		groundPlaneCollisionShape = nullptr;
	}

	if (pWorld->getNumCollisionObjects() != 0) {
		// Physics bodies were not cleaned up by the entity physics modules.
		// This occured when the app was closed by pressing the window "X" button and in a scenario that didn't use the zones system (battlezones_a_v1). In that case, the zone speciffic entities where never cleared.
		throw LogicException(LOC);
	}
	
	if (pWorld != nullptr) {
		delete pWorld;
		pWorld = nullptr;
	}

	if (solver != nullptr) {
		delete solver;
		solver = nullptr;
	}

	if (overlappingPairCache != nullptr) {
		delete overlappingPairCache;
		overlappingPairCache = nullptr;
	}

	if (dispatcher != nullptr) {
		delete dispatcher;
		dispatcher = nullptr;
	}

	if (constructionInfo != nullptr) {
		if (constructionInfo->m_persistentManifoldPool != nullptr) {
			constructionInfo->m_persistentManifoldPool->~btPoolAllocator();

			btAlignedFree(ptr_m_persistentManifoldPool);
			constructionInfo->m_persistentManifoldPool = nullptr;
		}
		
		delete constructionInfo;
		this->constructionInfo = nullptr;
	}
	if (collisionConfiguration != nullptr) {
		delete collisionConfiguration;
		this->collisionConfiguration = nullptr;
	}

	super::disposeMain();
}

GPP3D_Bullet::~GPP3D_Bullet() {
	//void
}
