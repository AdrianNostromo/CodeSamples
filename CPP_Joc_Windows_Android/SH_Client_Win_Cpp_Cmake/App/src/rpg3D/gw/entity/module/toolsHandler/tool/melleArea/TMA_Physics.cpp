#include "TMA_Physics.h"
#include <base/physics3D/util/Physics3DUtil.h>
#include <LinearMath/btPoolAllocator.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <rpg3D/gw/physics3D/IGamePartPhysics3D.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/config/ToolConfigMelleArea.h>
#include <base/physics3D/config/body/PBodyConfig.h>
#include <rpg3D/gw/entity/module/physics3D/pBodyUpdateHandler/DirectPBodyUpdateHandler.h>
#include <rpg3D/gw/physics3D/actor/PhysicsActor.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/entity/module/physics3D/util/PhysicsEntityCallbackFilter.h>
#include <base/math/util/BoolUtil.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/IPayloadReceiverTargetModule.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>

using namespace rpg3D;

TMA_Physics::TMA_Physics(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	pCollisionConfig = rpg3D::PhysicsCollissionConfig::Sword;
}

void TMA_Physics::onEntityChangePre() {
	super::onEntityChangePre();

	// Dispose pBOdy if exists.
	disposePBodyIfAny();
}

void TMA_Physics::onEntityChanged() {
	super::onEntityChanged();

	if (entity != nullptr) {
		if (pBody != nullptr) {
			throw LogicException(LOC);
		}

		rpg3D::IGamePartPhysics3D* gpPhysics3D = entity->getHandler()->getGW()->getComponentAs<rpg3D::IGamePartPhysics3D*>(true/*mustExist*/);
		pWorld = gpPhysics3D->getDynamicsWorld();

		pShape = physics3D::Physics3DUtil::BuildShape(config->triggerPBodyConfig->pShapeConfig);

		bool isDynamic = (config->triggerPBodyConfig->mass != 0.0f);

		{
			btTransform* physicsOffsetTransform_managed = nullptr;
			void* ptr_physicsOffsetTransform_managed = btAlignedAlloc(sizeof(btTransform), 16);
			physicsOffsetTransform_managed = new (ptr_physicsOffsetTransform_managed) btTransform();
			physicsOffsetTransform_managed->setIdentity();

			ptr_pBodyUpdateHandler = btAlignedAlloc(sizeof(DirectPBodyUpdateHandler), 16);
			pBodyUpdateHandler = new (ptr_pBodyUpdateHandler) DirectPBodyUpdateHandler(physicsOffsetTransform_managed, ptr_physicsOffsetTransform_managed);
		}

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) {
			pShape->calculateLocalInertia(config->triggerPBodyConfig->mass, localInertia);
		}

		pBody = physics3D::Physics3DUtil::BuildPBody(config->triggerPBodyConfig, config->triggerPBodyConfig->mass, nullptr/*pMotionState*/, pShape, localInertia);

		pBodyUpdateHandler->initPBody(pBody);

		{
			physicsActor = new rpg3D::PhysicsActor(entity, pCollisionConfig->groups, pCollisionConfig->anyInteraction_mask, pCollisionConfig->hardImpacts_mask);

			IPhysicsActor* p = physicsActor;
			pBody->setUserPointer(p);
		}
		{
			activeTargetUnitFilter = physicsActor->addEntityContactFilter(std::bind(&TMA_Physics::onTargetUnitContactChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			// Only test for units. This is a fast test and happens before the unit_speciffic_tests.
			activeTargetUnitFilter->setCollissionMaskBitMapOneWay(rpg3D::PhysicsCollissionConfig::Unit->groups)
				->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0,
					rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf,
					rpg3D::WorldOrEntityInteractionFilter::Flags::Team_Different,
					rpg3D::WorldOrEntityInteractionFilter::Flags::Alive
				))
				//->addEntityTag(rpg3D::EntityTags::UNIT, true)
				->addEntityModuleRequirement(rpg3D::IPayloadReceiverTargetModule::TYPE);
		}
	}
}

TMA_Physics::ImpactedEntityInfo* TMA_Physics::getContactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity) {
	for (ListDL<ImpactedEntityInfo>::Entry* entry = impactedEntitiesInfosList.getFirst(); entry != nullptr; entry = entry->next) {
		ImpactedEntityInfo& impactedEntityInfo = entry->data;

		if (impactedEntityInfo.wEntity == wEntity) {
			return &impactedEntityInfo;
		}
	}

	return nullptr;
}

ListDL<TMA_Physics::ImpactedEntityInfo>::Entry* TMA_Physics::getContactedEntityInfoListEntry(IWorldEntity* otherEntity) {
	for (ListDL<ImpactedEntityInfo>::Entry* entry = impactedEntitiesInfosList.getFirst(); entry != nullptr; entry = entry->next) {
		ImpactedEntityInfo& impactedEntityInfo = entry->data;

		if (impactedEntityInfo.wEntity->entity != nullptr && impactedEntityInfo.wEntity->entity == otherEntity) {
			return entry;
		}
	}

	return nullptr;
}

void TMA_Physics::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	//void
}

void TMA_Physics::disconnectFromGamePartsPre() {
	impactedEntitiesInfosList.clear();

	super::disconnectFromGamePartsPre();
}

void TMA_Physics::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	// Show or hide the sword visual. Keep the model in memory, just remove it from the display list.
	if (!isSelectedAndGWCapabilitiesExist) {
		// No need to cleanup physics stuff here because trigger cancel already occured before this point and is handled in ToolMelleArea and that does the cleanup.
	} else {
		//void
	}
}

void TMA_Physics::onTriggeringStart() {
	targetImpactsCount = 0;
	impactedEntitiesInfosList.clear();
}

void TMA_Physics::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (entity->getIsMarkedForRemoval()) {
		return;
	}

	if (impactedEntitiesInfosList.count() > 0) {
		// These entries are for OnlyFirstContact and for EachContact(if re-impactTimer is used).

		impactedEntitiesInfosList.enumerateDirect([this, deltaS](ListDL<ImpactedEntityInfo>::Entry* listEntry, ImpactedEntityInfo& impactedEntityInfo, bool& stopEnumeration) ->void {
			if (impactedEntityInfo.wEntity->entity == nullptr) {
				// Remove entries with nullptr entities.
				listEntry->remove();
			} else if (
				getLocalConfig()->reImpactOnPersistentContactCooldownS >= 0.0f/*re-impact_timer_used*/
				&& impactedEntityInfo.isInContact
				&& (
					getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::EachContact
					|| (
						getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact
						&& impactedEntityInfo.contactsCount <= 1/*still_on_first_contact*/
						)
					)
				)
			{
				// This is handled when re-impactTimer is used and it is used for each contact or only for the first contact and still on the first contact.

				impactedEntityInfo.reImpactTimerS += deltaS;

				if (impactedEntityInfo.reImpactTimerS >= getLocalConfig()->reImpactOnPersistentContactCooldownS) {
					// Cooldown passed and still in contact, do a re-impact.
					processTargetEntityImpact(impactedEntityInfo.wEntity->entity, true/*isSourceReImpactTimer*/);

					impactedEntityInfo.reImpactTimerS = 0.0f;
				}
			}
			});
	}
}

void TMA_Physics::onTargetUnitContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact) {
	if (!isContact) {
		if (getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::EachContact) {
			// Remove entry if EachContact is used, no need to keep thos because future contacts will impact again. Currently, this occurs when re-impactTimer is used.
			ListDL<TMA_Physics::ImpactedEntityInfo>::Entry* contactedEntityInfoListEntry = getContactedEntityInfoListEntry(otherEntity);
			if (contactedEntityInfoListEntry != nullptr) {
				contactedEntityInfoListEntry->remove();
			}
		} else if (getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact) {
			// OnlyFirstContact are kept to avoid fucture contact impacts.
			ListDL<TMA_Physics::ImpactedEntityInfo>::Entry* contactedEntityInfoListEntry = getContactedEntityInfoListEntry(otherEntity);
			if (contactedEntityInfoListEntry != nullptr) {
				contactedEntityInfoListEntry->data.isInContact = false;
			}
		}
	} else {
		ImpactedEntityInfo* impactedEntityInfo = processTargetEntityImpact(otherEntity, false/*isSourceReImpactTimer*/);
		if (impactedEntityInfo != nullptr) {
			impactedEntityInfo->isInContact = true;
		}
	}
}

TMA_Physics::ImpactedEntityInfo* TMA_Physics::processTargetEntityImpact(IWorldEntity* otherEntity, bool isSourceReImpactTimer) {
	if (entity->getIsMarkedForRemoval() || otherEntity->getIsMarkedForRemoval()) {
		// This projectile is removing, no longer transfers payloads.
		return nullptr;
	}

	if (config->maxTargetImpactsCount >= 0 && targetImpactsCount >= config->maxTargetImpactsCount) {
		// Maximum number of impacts reached.
		return nullptr;
	}

	ImpactedEntityInfo* impactedEntityInfo = getContactedEntityInfo(otherEntity->getWrapper());
	if (!isSourceReImpactTimer) {
		// Source is physics contact, handle it.
		if (getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact
			&& (impactedEntityInfo != nullptr && impactedEntityInfo->contactsCount > 0))
		{
			// Impact occurs only on first impact and target already impacted.

			// Still increment the contactsCount because it is used to disable the re-impactTimer logic because there should be not re-impacting on contacts after the first one.
			impactedEntityInfo->contactsCount++;

			return impactedEntityInfo;
		}
	}

	// Compute before targetImpactsCount is increased.
	// Do the decrease on re-impacts also. Mobs contact damage don't have this fallof so there is no problem of player units standing on top of mobs.
	float effectMultiplier = Math::max(
		config->minPayloadsValuesMultiplier,
		1.0f - config->payloadPercentDecreasePerTargetImpact * targetImpactsCount
	);

	targetImpactsCount++;

	if (!isSourceReImpactTimer) {
		// This is received from a physics contact it is a new one (the re-impactTimer already handles this).
		if ((getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::OnlyFirstContact
			|| (getLocalConfig()->impactAcknowledgeType == rpg3D::ToolConfigMelleArea::ImpactAcknowledgeType::EachContact && getLocalConfig()->reImpactOnPersistentContactCooldownS >= 0.0f)))
		{
			if (impactedEntityInfo == nullptr) {
				// Only save if OnlyFirstContact is used or if reImpactOnPersistentContactCooldownS >= 0.0f;
				ListDL<ImpactedEntityInfo>::Entry* entry = impactedEntitiesInfosList.appendEmplace(otherEntity->getWrapper());

				impactedEntityInfo = &entry->data;
			}

			impactedEntityInfo->contactsCount++;
			impactedEntityInfo->reImpactTimerS = 0.0f;
		}
	}

	if (impactSound != nullptr) {
		impactSound->play(config->impactSoundConfig_volume, config->impactSoundConfig_overlapCooldownS);
	}

	for (int i = 0; i < payloadsList.count(); i++) {
		std::shared_ptr<rpg3D::EffectPayload>& payloadEntry = payloadsList.getReference(i);

		payloadEntry->transferToEntity(entity, otherEntity, effectMultiplier);
	}

	return impactedEntityInfo;
}

void TMA_Physics::disposePBodyIfAny() {
	// Remove physics objects.
	if (pBody != nullptr) {
		pBody->setUserPointer(nullptr);

		if (pBody->isInWorld()) {
			pWorld->removeRigidBody(pBody);
		}

		delete pBody;
		pBody = nullptr;
	}

	if (pShape != nullptr) {
		delete pShape;
		pShape = nullptr;
	}

	// Unregister the activeTargetUnitFilter if active.
	if (activeTargetUnitFilter != nullptr) {
		physicsActor->removeEntityContactFilter(activeTargetUnitFilter);

		activeTargetUnitFilter = nullptr;
	}

	//asd_1;// this is supposed to be in disposePre for modules, it requires modules. maybe refactor code so that this call can be removed.
	if (physicsActor != nullptr) {
		physicsActor->reservedDisposePre();
	}
	if (physicsActor != nullptr) {
		physicsActor->reservedDisposeMain();
		delete physicsActor;
		physicsActor = nullptr;
	}

	if (pBodyUpdateHandler != nullptr) {
		pBodyUpdateHandler->~IPBodyUpdateHandler();
		btAlignedFree(ptr_pBodyUpdateHandler);
		ptr_pBodyUpdateHandler = nullptr;
		pBodyUpdateHandler = nullptr;
	}

	pWorld = nullptr;
}

void TMA_Physics::disposeMain() {
	// Dispose pBOdy if exists.
	disposePBodyIfAny();

	super::disposeMain();
}

TMA_Physics::~TMA_Physics() {
	//void
}
