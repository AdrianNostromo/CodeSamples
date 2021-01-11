#include "P3DM_Body.h"
#include <base/math/Math.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include <rpg3D/gw/entity/template/physics3D/pActorController/CfgCharacterPActorController.h>
#include <rpg3D/gw/physics3D/IGamePartPhysics3D.h>
#include <base/exceptions/NotImplementedException.h>
#include "pBodyUpdateHandler/CallbackMotionStatePBodyUpdateHandler.h"
#include "pBodyUpdateHandler/DirectPBodyUpdateHandler.h"
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>
#include "pActorController/CharacterPActorController.h"
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_LinearX.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/math/Quaternion.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <rpg3D/gw/entity/template/physics3D/constraint/PhysicsConstraintConfigGeneric6Dof.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/physics3D/util/Physics3DUtil.h>

using namespace rpg3D;

P3DM_Body::P3DM_Body(
	IWorldEntity* entity,
	Physics3DTemplate* t,
	int physicsGroupsBitMap,
	int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	: super(entity, t),
	physicsGroupsBitMap(physicsGroupsBitMap),
	anyInteraction_maskBitMap(anyInteraction_maskBitMap), hardImpact_maskBitMap(hardImpact_maskBitMap)
{
	//void
}

void P3DM_Body::createMain() {
	super::createMain();

	for (int i = 0; i < t->pBodiesConfigsList.count(); i++) {
		rpg3D::PBodyConfig* pBodyConfig = t->pBodiesConfigsList.getPointer(i);

		btCollisionShape* pShape = physics3D::Physics3DUtil::BuildShape(pBodyConfig->pShapeConfig, tilingInformation);
		
		if (cachedScale.x != 1.0f || cachedScale.y != 1.0f || cachedScale.z != 1.0f) {
			btVector3 v{ cachedScale.x, cachedScale.y, cachedScale.z };
			pShape->setLocalScaling(v);
		}

		btVector3 localInertia(0, 0, 0);
		float scaledMass = syncSinglePBodyMass(nullptr/*pBody*/, pShape, pBodyConfig, localInertia);

		void* ptr_physicsOffsetTransform_managed = nullptr;
		btTransform* physicsOffsetTransform_managed = nullptr;
		if (pBodyConfig->physicsBodyOffsetPos != nullptr || pBodyConfig->physicsBodyOffsetRot != nullptr) {
			void* mem = btAlignedAlloc(sizeof(btTransform), 16);
			ptr_physicsOffsetTransform_managed = mem;
			physicsOffsetTransform_managed = new (mem) btTransform();
			physicsOffsetTransform_managed->setIdentity();

			if (pBodyConfig->physicsBodyOffsetPos != nullptr) {
				btVector3 p{ pBodyConfig->physicsBodyOffsetPos->x, pBodyConfig->physicsBodyOffsetPos->y, pBodyConfig->physicsBodyOffsetPos->z };
				physicsOffsetTransform_managed->setOrigin(p);
			}
			if (pBodyConfig->physicsBodyOffsetRot != nullptr) {
				btQuaternion q{ pBodyConfig->physicsBodyOffsetRot->x, pBodyConfig->physicsBodyOffsetRot->y, pBodyConfig->physicsBodyOffsetRot->z, pBodyConfig->physicsBodyOffsetRot->w };
				physicsOffsetTransform_managed->setRotation(q);
			}
		}

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		// Motion steps are used only for dyanmics.
		CallbackMotionStatePBodyUpdateHandler* cPMotionState = nullptr;
		IPBodyUpdateHandler* pBodyUpdateHandler = nullptr;
		void* ptr_pBodyUpdateHandler = nullptr;
		if (pBodyConfig->mass > 0.0f) {
			void* mem = btAlignedAlloc(sizeof(CallbackMotionStatePBodyUpdateHandler), 16);
			ptr_pBodyUpdateHandler = mem;
			if ((pBodyConfig->ussageBitMask & rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate) != 0) {
				cPMotionState = new (mem) CallbackMotionStatePBodyUpdateHandler(physicsOffsetTransform_managed, ptr_physicsOffsetTransform_managed, std::bind(&P3DM_Body::onMotionStateTransformChanged, this));
			} else {
				cPMotionState = new (mem) CallbackMotionStatePBodyUpdateHandler(physicsOffsetTransform_managed, ptr_physicsOffsetTransform_managed, nullptr);
			}
			pBodyUpdateHandler = cPMotionState;
		} else {
			void* mem = btAlignedAlloc(sizeof(DirectPBodyUpdateHandler), 16);
			ptr_pBodyUpdateHandler = mem;
			pBodyUpdateHandler = new (mem) DirectPBodyUpdateHandler(physicsOffsetTransform_managed, ptr_physicsOffsetTransform_managed);
		}

		btRigidBody* pBody = physics3D::Physics3DUtil::BuildPBody(pBodyConfig, scaledMass, cPMotionState, pShape, localInertia);
		
		pBodyUpdateHandler->initPBody(pBody);
		btVector3 p2{ cachedPos.x, cachedPos.y, cachedPos.z };
		pBodyUpdateHandler->setVisualPos(p2);
		btQuaternion q2{ cachedRot.x, cachedRot.y, cachedRot.z, cachedRot.w };
		pBodyUpdateHandler->setVisualRot(q2);

		pBodiesConfiguredList.append_emplace(
			pBodyConfig, pBody, pShape,
			pBodyUpdateHandler, ptr_pBodyUpdateHandler
		);
	}

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		if ((configuredPBody->config->ussageBitMask & rpg3D::PBodyConfig::UssageBitGroups::Movement) != 0) {
			if (pBodyMovement != nullptr || pConfiguredBodyMovement != nullptr) {
				throw LogicException(LOC);
			}

			pConfiguredBodyMovement = configuredPBody;
			pBodyMovement = configuredPBody->pBody;
		}
		if ((configuredPBody->config->ussageBitMask & rpg3D::PBodyConfig::UssageBitGroups::CardinalRotation) != 0) {
			if (pBodyCardinalRotation != nullptr || pConfiguredBodyCardinalRotation != nullptr) {
				throw LogicException(LOC);
			}

			pConfiguredBodyCardinalRotation = configuredPBody;
			pBodyCardinalRotation = configuredPBody->pBody;
		}
		if ((configuredPBody->config->ussageBitMask & rpg3D::PBodyConfig::UssageBitGroups::VisualsUpdate) != 0) {
			if (pBodyVisualsAnchor != nullptr || pConfiguredBodyVisualsAnchor != nullptr) {
				throw LogicException(LOC);
			}

			pConfiguredBodyVisualsAnchor = configuredPBody;
			pBodyVisualsAnchor = configuredPBody->pBody;
		}
	}
	if (pBodyMovement == nullptr || pConfiguredBodyCardinalRotation == nullptr || pBodyVisualsAnchor == nullptr) {
		throw LogicException(LOC);
	}

	if (t->pActorControllerCfg != nullptr) {
		if (t->pActorControllerCfg->getType() == CfgCharacterPActorController::TYPE) {
			CfgCharacterPActorController* cPActorControllerCfg = t->pActorControllerCfg->castToSubtype<CfgCharacterPActorController*>(CfgCharacterPActorController::TYPE);

			void* mem = btAlignedAlloc(sizeof(CharacterPActorController), 16);
			ptr_pAction = pAction = new (mem) CharacterPActorController(cPActorControllerCfg, pConfiguredBodyMovement->pBodyUpdateHandler);
		} else {
			// Character 
			throw LogicException(LOC);
		}
	}

	for (int i = 0; i < t->pConstraintsConfigsList.count(); i++) {
		PhysicsConstraintConfig* pConstraintsConfig = t->pConstraintsConfigsList.getDirect(i);

		btTypedConstraint* pConstraint = CreateConstraint(pConstraintsConfig, pBodiesConfiguredList);

		pConfiguredConstraintsList.append_emplace(pConstraintsConfig, pConstraint);
	}
}

void P3DM_Body::createBMain() {
	super::createBMain();

	if (e->getIsInGameWorldAndActive()) {
		rpg3D::IGamePartPhysics3D* gpPhysics3D = entity->getHandler()->getGW()->getComponentAs<rpg3D::IGamePartPhysics3D*>(true/*mustExist*/);
		pWorld = gpPhysics3D->getDynamicsWorld();

		for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

			pWorld->addRigidBody(configuredPBody->pBody, physicsGroupsBitMap, anyInteraction_maskBitMap);

			if (configuredPBody->config->customGravity != nullptr) {
				btVector3 v{ configuredPBody->config->customGravity->x, configuredPBody->config->customGravity->y, configuredPBody->config->customGravity->z };
				// This must be called after the physics body is added to the pWorld or it wont work.
				configuredPBody->pBody->setGravity(v);
			}
		}

		for (int i = 0; i < pConfiguredConstraintsList.count(); i++) {
			ConfiguredConstraint* pConfiguredConstraint = pConfiguredConstraintsList.getPointer(i);

			pWorld->addConstraint(pConfiguredConstraint->pConstraint, pConfiguredConstraint->config->disableCollisionsBetweenLinkedBodies);
			pConfiguredConstraint->isInPWorld = true;
		}

		if (pAction != nullptr) {
			pAction->setPWorld(pWorld);
			pWorld->addAction(pAction);
		}
	}

	if (!isPositionalSyncListenerInitialised) {
		// The sync listeners were not initialised;
		throw LogicException(LOC);
	}
}

void P3DM_Body::setTilingInformation(std::shared_ptr<graphics::TilingInformation> tilingInformation) {
	this->tilingInformation = tilingInformation;

	if (pBodiesConfiguredList.count() <= 0) {
		return;
	}

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		btCollisionShape* old_pShape = configuredPBody->pShape;
		configuredPBody->pShape = nullptr;

		bool isInPWorld = configuredPBody->pBody->isInWorld();
		if (isInPWorld) {
			// This can also be achieved with removing and adding the pBody to the pWorld.
			/// This method avoids that and has some benefits it seems (Not sure what, maybe that takes more code_run_time).
			/// Without using any of this, objects that are in contact with this pBody will not detect the change even if moving on it.

			//pWorld->removeRigidBody(configuredPBody->pBody);
		}

		configuredPBody->pShape = physics3D::Physics3DUtil::BuildShape(configuredPBody->config->pShapeConfig, tilingInformation);
		configuredPBody->pBody->setCollisionShape(configuredPBody->pShape);

		if (old_pShape != nullptr) {
			delete old_pShape;
		}

		// Not sure if needed but sync some things;
		btVector3 localInertia(0, 0, 0);
		syncSinglePBodyMass(configuredPBody->pBody, configuredPBody->pShape, configuredPBody->config, localInertia);

		if (isInPWorld) {
			//pWorld->addRigidBody(configuredPBody->pBody, physicsGroupsBitMap, anyInteraction_maskBitMap);

			// This can also be achieved with removing and adding the pBody to the pWorld.
			/// This method avoids that and has some benefits it seems (Not sure what, maybe that takes more code_run_time).
			/// Without using any of this, objects that are in contact with this pBody will not detect the change even if moving on it.
			btCollisionObjectArray& obarr = pWorld->getCollisionObjectArray();
			int obarr_count = obarr.size();
			for (int i = 0; i < obarr_count; i++) {
				btCollisionObject* ob = obarr[i];
				if (ob->getCollisionShape() == configuredPBody->pShape) {
					btBroadphaseProxy* proxy = obarr[i]->getBroadphaseHandle();

					if (proxy)
						pWorld->getPairCache()->cleanProxyFromPairs(proxy, pWorld->getDispatcher());
				}
			}
		}
	}
}

void P3DM_Body::setMassMultiplier(float massMultiplier) {
	if (this->massMultiplier == massMultiplier) {
		return;
	}
	this->massMultiplier = massMultiplier;
	
	syncMass();
}

void P3DM_Body::setIsKinematicallyFrozen(bool isKinematicallyFrozen) {
	if (this->isKinematicallyFrozen == isKinematicallyFrozen) {
		return;
	}
	this->isKinematicallyFrozen = isKinematicallyFrozen;

	if (this->isKinematicallyFrozen) {
		// Stop all forces.
		clearAllForces();
	}

	syncMass();
}

btScalar P3DM_Body::syncMass() {
	float totalMass = 0.0f;

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		btVector3 localInertia(0, 0, 0);
		totalMass += syncSinglePBodyMass(configuredPBody->pBody, configuredPBody->pShape, configuredPBody->config, localInertia);
	}

	return totalMass;
}

float P3DM_Body::syncSinglePBodyMass(btRigidBody* pBody/*pBody*/, btCollisionShape* pShape, rpg3D::PBodyConfig* pBodyConfig, btVector3& localInertia) {
	btScalar mass(isKinematicallyFrozen ? 0.0f : pBodyConfig->mass * this->massMultiplier);

	if (pBodyConfig->dynamicsType != rpg3D::PBodyConfig::DynamicsType::Dynamic && isKinematicallyFrozen) {
		// Only dynamic pBodies can be kinematically frozen.
		throw LogicException(LOC);
	}

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	if (isDynamic) {
		pShape->calculateLocalInertia(mass, localInertia);
	}

	if (pBody != nullptr) {
		// Set the kinematic flag as needed.
		if (!this->isKinematicallyFrozen) {
			// Remove flag.
			pBody->setCollisionFlags(pBody->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
		} else {
			// Set flag.
			pBody->setCollisionFlags(pBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		}

		pBody->setMassProps(mass, localInertia);

		if (pBody->getActivationState() != ACTIVE_TAG) {
			// Param true is required because ISLAND_SLEEPING pBodies will not be activated otherwise.
			pBody->activate(true);
		}
	}

	return mass;
}

void P3DM_Body::onMotionStateTransformChanged() {
	// This is only the one received from the visuals pBody, not for the movement pBody;

	btTransform t = getPBodyVisualTransform();
	btVector3& pos = t.getOrigin();
	btQuaternion rot = t.getRotation();
	if (!cachedPos.equals(pos.x(), pos.y(), pos.z(), Math::FLOAT_ROUNDING_ERROR)) {
		cachedPos.set(pos.x(), pos.y(), pos.z());

		e->dispatchEvent(*IWorldEntity::EVENT_syncPos);
	}
	if (!Math::isEqual(cachedRot.x, rot.x(), Math::FLOAT_ROUNDING_ERROR)
		|| !Math::isEqual(cachedRot.y, rot.y(), Math::FLOAT_ROUNDING_ERROR)
		|| !Math::isEqual(cachedRot.z, rot.z(), Math::FLOAT_ROUNDING_ERROR)
		|| !Math::isEqual(cachedRot.w, rot.w(), Math::FLOAT_ROUNDING_ERROR))
	{
		cachedRot.set(rot.x(), rot.y(), rot.z(), rot.w());
		
		e->dispatchEvent(*IWorldEntity::EVENT_syncRot);
	}
}

btTypedConstraint* P3DM_Body::CreateConstraint(PhysicsConstraintConfig* pConstraintsConfig, ArrayList<physics3D::ConfiguredPBody>& pBodiesConfiguredList) {
	btTypedConstraint* pConstraint;
	if (pConstraintsConfig->type == PhysicsConstraintConfigGeneric6Dof::TYPE) {
		PhysicsConstraintConfigGeneric6Dof* cPConstraintsConfig = pConstraintsConfig->getAs<PhysicsConstraintConfigGeneric6Dof*>(PhysicsConstraintConfigGeneric6Dof::TYPE);

		btRigidBody* pBodyA = pBodiesConfiguredList.getReference(cPConstraintsConfig->pBodyIndexA).pBody;
		btRigidBody* pBodyB = pBodiesConfiguredList.getReference(cPConstraintsConfig->pBodyIndexB).pBody;

		btTransform frameInA;
		frameInA.setIdentity();
		frameInA.setOrigin(btVector3(
			cPConstraintsConfig->framePosInA.x,
			cPConstraintsConfig->framePosInA.y,
			cPConstraintsConfig->framePosInA.z
		));

		btTransform frameInB;
		frameInB.setIdentity();
		frameInB.setOrigin(btVector3(
			cPConstraintsConfig->framePosInB.x,
			cPConstraintsConfig->framePosInB.y,
			cPConstraintsConfig->framePosInB.z
		));

		btGeneric6DofConstraint* cPConstraint = new btGeneric6DofConstraint(*pBodyA, *pBodyB, frameInA, frameInB, true/*useLinearReferenceFrameA*/);
		pConstraint = cPConstraint;
		
		// Linear limits.
		// NOTE: fully enabled. cPConstraint->setLimit(..., -SIMD_INFINITY, SIMD_INFINITY);
		// NOTE: fully locked. cPConstraint->setLimit(..., 0, 0);
		if (cPConstraintsConfig->limitPosX.type != PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom || cPConstraintsConfig->limitPosY.type != PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom || cPConstraintsConfig->limitPosZ.type != PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom) {
			throw LogicException(LOC);
		}
		cPConstraint->setLimit(0, cPConstraintsConfig->limitPosX.customMin, cPConstraintsConfig->limitPosX.customMax);
		cPConstraint->setLimit(1, cPConstraintsConfig->limitPosY.customMin, cPConstraintsConfig->limitPosY.customMax);
		cPConstraint->setLimit(2, cPConstraintsConfig->limitPosZ.customMin, cPConstraintsConfig->limitPosZ.customMax);

		// Angle limits.
		// Fully enabled rotation: cPConstraint->setLimit(..., -SIMD_PI, SIMD_PI);
		if (cPConstraintsConfig->limitAngX.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom) {
			cPConstraint->setLimit(3, cPConstraintsConfig->limitAngX.customMin, cPConstraintsConfig->limitAngX.customMax);
		} else if (cPConstraintsConfig->limitAngX.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning) {
			cPConstraint->setLimit(3, -SIMD_PI, SIMD_PI);
		} else {
			throw LogicException(LOC);
		}
		if (cPConstraintsConfig->limitAngY.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom) {
			cPConstraint->setLimit(4, cPConstraintsConfig->limitAngY.customMin, cPConstraintsConfig->limitAngY.customMax);
		} else if (cPConstraintsConfig->limitAngY.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning) {
			cPConstraint->setLimit(4, -SIMD_PI, SIMD_PI);
		} else {
			throw LogicException(LOC);
		}
		if (cPConstraintsConfig->limitAngZ.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::Custom) {
			cPConstraint->setLimit(5, cPConstraintsConfig->limitAngZ.customMin, cPConstraintsConfig->limitAngZ.customMax);
		} else if (cPConstraintsConfig->limitAngZ.type == PhysicsConstraintConfigGeneric6Dof::Limit::Type::FreeSpinning) {
			cPConstraint->setLimit(5, -SIMD_PI, SIMD_PI);
		} else {
			throw LogicException(LOC);
		}
	} else {
		throw LogicException(LOC);
	}

	return pConstraint;
}

void P3DM_Body::setIsPositionalSyncListenerEnabled(bool isPositionalSyncListenerEnabled) {
	if (!isPositionalSyncListenerInitialised || this->isPositionalSyncListenerEnabled != isPositionalSyncListenerEnabled) {
		this->isPositionalSyncListenerEnabled = isPositionalSyncListenerEnabled;
		isPositionalSyncListenerInitialised = true;

		if (!this->isPositionalSyncListenerEnabled) {
			syncAutoListenersList.clear();
		} else {
			e->addEventListener(
				IWorldEntity::EVENT_syncPos->type, IWorldEntity::EVENT_syncRot->type, IWorldEntity::EVENT_syncCardinalRotation->type,
				std::bind(&P3DM_Body::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				syncAutoListenersList
			);
			e->addEventListener(
				IWorldEntity::EVENT_syncScale->type,
				std::bind(&P3DM_Body::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				syncAutoListenersList
			);
		}
	}
}

void P3DM_Body::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	rpg3D::IGamePartPhysics3D* gpPhysics3D = entity->getHandler()->getGW()->getComponentAs<rpg3D::IGamePartPhysics3D*>(true/*mustExist*/);
	pWorld = gpPhysics3D->getDynamicsWorld();

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		pWorld->addRigidBody(configuredPBody->pBody, physicsGroupsBitMap, anyInteraction_maskBitMap);

		if (configuredPBody->config->customGravity != nullptr) {
			btVector3 v{ configuredPBody->config->customGravity->x, configuredPBody->config->customGravity->y, configuredPBody->config->customGravity->z };
			// This must be called after the physics body is added to the pWorld or it wont work.
			configuredPBody->pBody->setGravity(v);
		}
	}

	for (int i = 0; i < pConfiguredConstraintsList.count(); i++) {
		ConfiguredConstraint* pConfiguredConstraint = pConfiguredConstraintsList.getPointer(i);

		pWorld->addConstraint(pConfiguredConstraint->pConstraint, pConfiguredConstraint->config->disableCollisionsBetweenLinkedBodies);
		pConfiguredConstraint->isInPWorld = true;
	}

	if (pAction != nullptr) {
		pAction->setPWorld(pWorld);
		pWorld->addAction(pAction);
	}
}

void P3DM_Body::disconnectFromGamePartsPre() {
	if (pAction != nullptr) {
		if (pAction->getPWorld() != nullptr) {
			pAction->getPWorld()->removeAction(pAction);
			pAction->setPWorld(nullptr);
		}
	}

	for (int i = 0; i < pConfiguredConstraintsList.count(); i++) {
		ConfiguredConstraint* pConfiguredConstraint = pConfiguredConstraintsList.getPointer(i);

		if (pConfiguredConstraint->isInPWorld) {
			pConfiguredConstraint->isInPWorld = false;
			pWorld->removeConstraint(pConfiguredConstraint->pConstraint);
		}
	}

	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		if (configuredPBody->pBody->isInWorld()) {
			// Remove the physics actor so there will be no contact break
			/// because if this entity is disposed, there will be a invaid contact break without an actor and that is processed as an error currently.
			pWorld->removeRigidBody(configuredPBody->pBody);
		}
	}

	pWorld = nullptr;

	super::disconnectFromGamePartsPre();
}

void P3DM_Body::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_syncPos->type) {
		syncPos(entity->getPos());
	} else if (event.type == IWorldEntity::EVENT_syncRot->type) {
		syncRot(*entity->getRot());
	} else if (event.type == IWorldEntity::EVENT_syncCardinalRotation->type) {
		syncCardinalRotation(entity->getRot()->getAngleAround(Vector3::Z));
	} else if (event.type == IWorldEntity::EVENT_syncScale->type) {
		syncScale(entity->getScale());
	} else {
		throw LogicException(LOC);
	}
}

void P3DM_Body::syncPos(Vector3* pos) {
	syncPos(pos->x, pos->y, pos->z);
}

void P3DM_Body::syncPos(float x, float y, float z) {
	btVector3 p(x, y, z);
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualPos(p);
	}
}

void P3DM_Body::syncPosX(float x) {
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualPosX(x);
	}
}

void P3DM_Body::syncPosY(float y) {
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualPosY(y);
	}
}

void P3DM_Body::syncPosZ(float z) {
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualPosZ(z);
	}
}

void P3DM_Body::syncRot(Quaternion& _rot) {
	btQuaternion q{ _rot.x, _rot.y , _rot.z , _rot.w };
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualRot(q);
	}
}

void P3DM_Body::syncRot(Vector3& _axis, float degrees) {
	btVector3 axis{ _axis.x, _axis.y, _axis.z };
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualRot(axis, degrees);
	}
}

void P3DM_Body::syncRotEulerZYX(Vector3& _axis, float degrees) {
	btVector3 axis{ _axis.x, _axis.y, _axis.z };
	
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		configuredPBody->pBodyUpdateHandler->setVisualRotOnSingleAxis(axis, degrees);
	}
}

void P3DM_Body::syncCardinalRotation(float degrees) {
	btVector3 axis{ 0.0f, 0.0f, 1.0f };

	pConfiguredBodyCardinalRotation->pBodyUpdateHandler->setVisualRotOnSingleAxis(axis, degrees);
}

void P3DM_Body::syncScale(Vector3* scale) {
	if (this->cachedScale.equals(scale)) {
		return;
	}
	this->cachedScale.set(scale);

	activateAllPBodies();

	btVector3 v{ cachedScale.x, cachedScale.y, cachedScale.z };

	if (pWorld != nullptr) {
		for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

			configuredPBody->pShape->setLocalScaling(v);

			if (configuredPBody->pBody->isInWorld()) {
				// This is to get the new bounding box of the scale to take effect. Static objects don't update their AABB for performance reasons. This is called to avoid removing and re-adding the body to the pWorld.
				pWorld->updateSingleAabb(configuredPBody->pBody);
			}
		}
	}
}

void P3DM_Body::activateAllPBodies() {
	for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
	    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

		if (configuredPBody->pBody->getActivationState() != ACTIVE_TAG) {
			configuredPBody->pBody->activate(true);
		}
	}
}

btRigidBody* P3DM_Body::getPBodyVisualsAnchor() {
	return pBodyVisualsAnchor;
}

btRigidBody* P3DM_Body::getPBodyMovement() {
	return pBodyMovement;
}

btTransform P3DM_Body::getPBodyVisualTransform() {
	if (pConfiguredBodyVisualsAnchor->pBodyUpdateHandler == nullptr) {
		throw LogicException(LOC);
	}
	
	return pConfiguredBodyVisualsAnchor->pBodyUpdateHandler->getVisualWorldTransform();
}

void P3DM_Body::disposePre() {
	if (pAction != nullptr) {
		if (pAction->getPWorld() != nullptr) {
			pAction->getPWorld()->removeAction(pAction);
			pAction->setPWorld(nullptr);
		}

		pAction->~IPActorControllerBase();
		btAlignedFree(ptr_pAction);
		pAction = nullptr;
	}

	if (pConfiguredConstraintsList.count() > 0) {
		for (int i = 0; i < pConfiguredConstraintsList.count(); i++) {
			ConfiguredConstraint* pConfiguredConstraint = pConfiguredConstraintsList.getPointer(i);

			if (pConfiguredConstraint->isInPWorld) {
				pConfiguredConstraint->isInPWorld = false;
				pWorld->removeConstraint(pConfiguredConstraint->pConstraint);
			}
		}

		pConfiguredConstraintsList.clear();
	}

	if (pBodiesConfiguredList.count() > 0) {
		for (int i = 0; i < pBodiesConfiguredList.count(); i++) {
		    physics3D::ConfiguredPBody* configuredPBody = pBodiesConfiguredList.getPointer(i);

			if (configuredPBody->pBodyUpdateHandler != nullptr) {
				configuredPBody->pBodyUpdateHandler->~IPBodyUpdateHandler();
				btAlignedFree(configuredPBody->ptr_pBodyUpdateHandler);
				configuredPBody->ptr_pBodyUpdateHandler = nullptr;
				configuredPBody->pBodyUpdateHandler = nullptr;
			}

			if (configuredPBody->pBody->isInWorld()) {
				pWorld->removeRigidBody(configuredPBody->pBody);
			}

			delete configuredPBody->pBody;
			configuredPBody->pBody = nullptr;

			if (configuredPBody->pShape != nullptr) {
				delete configuredPBody->pShape;
				configuredPBody->pShape = nullptr;
			}
		}
		pBodiesConfiguredList.clear();
	}

	pWorld = nullptr;

	super::disposePre();
}

void P3DM_Body::disposeMain() {
	syncAutoListenersList.clear();

	super::disposeMain();
}

P3DM_Body::~P3DM_Body() {
	//void
}
