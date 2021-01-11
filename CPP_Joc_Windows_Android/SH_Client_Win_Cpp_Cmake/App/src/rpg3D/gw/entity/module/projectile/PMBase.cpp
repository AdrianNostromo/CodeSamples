#include "PMBase.h"
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/module/payloadReceiverTarget/IPayloadReceiverTargetModule.h>
#include <rpg3D/gw/entity/module/statusEffects/StatusEffectsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <base/math/util/BoolUtil.h>
#include <base/math/Math.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

using namespace rpg3D;

EntityRemoveReason *const PMBase::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

PMBase::PMBase(
	IWorldEntity* entity,
	ProjectileTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulation->type,
		std::bind(&PMBase::onGWEvent_gameLoop_simulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* PMBase::peekPayloadsList() {
    return &payloadsList;
}

void PMBase::addPayload(std::shared_ptr<rpg3D::EffectPayload> payload) {
	this->payloadsList.appendReference(payload);
}

void PMBase::addPayloadsList(ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList) {
	for(int i=0;i<payloadsList->count();i++) {
		std::shared_ptr<rpg3D::EffectPayload>& entry = payloadsList->getReference(i);

		addPayload(entry);
	}
}

void PMBase::createBMain() {
    super::createBMain();

	team = e->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(true/*mustExist*/);

	activeTargetUnitFilter = physics3DModule->addEntityContactFilter(std::bind(&PMBase::onTargetUnitContactChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// Only test for units. This is a fast test and happens before the unit_speciffic_tests.
	activeTargetUnitFilter->setCollissionMaskBitMapOneWay(rpg3D::PhysicsCollissionConfig::Unit->groups)
		->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, 
			rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf, 
			rpg3D::WorldOrEntityInteractionFilter::Flags::Team_Different, 
			rpg3D::WorldOrEntityInteractionFilter::Flags::Alive
		))
		//->addEntityTag(rpg3D::EntityTags::UNIT, true)
		->addEntityModuleRequirement(rpg3D::IPayloadReceiverTargetModule::TYPE);
	
	if (t->maxEnvironmentImpactsCount >= 1 || t->freezeMovementOnEnvironmentImpact) {
		// Auto removal used. The projectile will be removed if it hits the environment t->activeEnvironmentContactFilter times.

		activeEnvironmentContactFilter = physics3DModule->addEntityContactFilter(std::bind(&PMBase::onEnvironmentContactChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		// Only test for units. This is a fast test and happens before the unit_speciffic_tests.
		activeEnvironmentContactFilter->setCollissionMaskBitMapOneWay(rpg3D::PhysicsCollissionConfig::Environment->groups);
			//->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf, rpg3D::WorldOrEntityInteractionFilter::Flags::Team_Different))
			//->addEntityTag(rpg3D::EntityTags::UNIT, true)
			//->addEntityModuleRequirement(rpg3D::IPayloadReceiverTargetModule::TYPE);
	}
}


void PMBase::onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event) {
	if (entity->getIsMarkedForRemoval()) {
		return;
	}

	if (impactedEntitiesInfosList.count() > 0) {
		// These entries are for OnlyFirstContact and for EachContact(if re-impactTimer is used).

		float deltaS = event.deltaS;
		impactedEntitiesInfosList.enumerateDirect([this, deltaS](ListDL<ImpactedEntityInfo>::Entry* listEntry, ImpactedEntityInfo& impactedEntityInfo, bool& stopEnumeration) ->void {
			if (impactedEntityInfo.wEntity->entity == nullptr) {
				// Remove entries with nullptr entities.
				listEntry->remove();
			} else if (
				t->reImpactOnPersistentContactCooldownS >= 0.0f/*re-impact_timer_used*/ 
				&& impactedEntityInfo.isInContact
				&& (
					t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::EachContact 
					|| (
						t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::OnlyFirstContact 
						&& impactedEntityInfo.contactsCount <= 1/*still_on_first_contact*/
						)
					)
				)
			{
				// This is handled when re-impactTimer is used and it is used for each contact or only for the first contact and still on the first contact.

				impactedEntityInfo.reImpactTimerS += deltaS;

				if (impactedEntityInfo.reImpactTimerS >= t->reImpactOnPersistentContactCooldownS) {
					// Cooldown passed and still in contact, do a re-impact.
					processTargetEntityImpact(impactedEntityInfo.wEntity->entity, true/*isSourceReImpactTimer*/);

					impactedEntityInfo.reImpactTimerS = 0.0f;
				}
			}
		});
	}
}

PMBase::ImpactedEntityInfo* PMBase::getContactedEntityInfo(std::shared_ptr<base::WrappedWorldEntity> wEntity) {
	for (ListDL<ImpactedEntityInfo>::Entry* entry = impactedEntitiesInfosList.getFirst(); entry != nullptr; entry = entry->next) {
		ImpactedEntityInfo& impactedEntityInfo = entry->data;

		if (impactedEntityInfo.wEntity == wEntity) {
			return &impactedEntityInfo;
		}
	}

	return nullptr;
}

ListDL<PMBase::ImpactedEntityInfo>::Entry* PMBase::getContactedEntityInfoListEntry(IWorldEntity* otherEntity) {
	for (ListDL<ImpactedEntityInfo>::Entry* entry = impactedEntitiesInfosList.getFirst(); entry != nullptr; entry = entry->next) {
		ImpactedEntityInfo& impactedEntityInfo = entry->data;

		if (impactedEntityInfo.wEntity->entity != nullptr && impactedEntityInfo.wEntity->entity == otherEntity) {
			return entry;
		}
	}

	return nullptr;
}

void PMBase::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);

	if (t->impactSoundConfig_id != nullptr) {
		impactSound = entity->getGW()->getHandler()->getAppAssets()->getEmbeddedAssetsManager()->getSound(*t->impactSoundConfig_id);
		if (impactSound == nullptr) {
			throw LogicException(LOC);
		}
	}
}

void PMBase::disconnectFromGamePartsPre() {
	impactedEntitiesInfosList.clear();

	super::disconnectFromGamePartsPre();
}

void PMBase::onTargetUnitContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact) {
	if (!isContact) {
		if (t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::EachContact) {
			// Remove entry if EachContact is used, no need to keep thos because future contacts will impact again. Currently, this occurs when re-impactTimer is used.
			ListDL<PMBase::ImpactedEntityInfo>::Entry* contactedEntityInfoListEntry = getContactedEntityInfoListEntry(otherEntity);
			if (contactedEntityInfoListEntry != nullptr) {
				contactedEntityInfoListEntry->remove();
			}
		}else if (t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::OnlyFirstContact) {
			// OnlyFirstContact are kept to avoid fucture contact impacts.
			ListDL<PMBase::ImpactedEntityInfo>::Entry* contactedEntityInfoListEntry = getContactedEntityInfoListEntry(otherEntity);
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

PMBase::ImpactedEntityInfo* PMBase::processTargetEntityImpact(IWorldEntity* otherEntity, bool isSourceReImpactTimer) {
	if (e->getIsMarkedForRemoval() || otherEntity->getIsMarkedForRemoval()) {
		// This projectile is removing, no longer transfers payloads.
		return nullptr;
	}
	if (t->disablePayloadImpactsOnEnvironmentImpact && environmentImpactsCount > 0) {
		return nullptr;
	}

	ImpactedEntityInfo* impactedEntityInfo = getContactedEntityInfo(otherEntity->getWrapper());
	if (!isSourceReImpactTimer) {
		// Source is physics contact, handle it.
		if (t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::OnlyFirstContact
			&& (impactedEntityInfo != nullptr && impactedEntityInfo->contactsCount > 0))
		{
			// Impact occurs only on first impact and target already impacted.

			// Still increment the contactsCount because it is used to disable the re-impactTimer logic because there should be not re-impacting on contacts after the first one.
			impactedEntityInfo->contactsCount++;

			return impactedEntityInfo;
		}
	}

	if (impactSound != nullptr) {
		impactSound->play(t->impactSoundConfig_volume, t->impactSoundConfig_overlapCooldownS);
	}

	// Compute before targetImpactsCount is increased.
	// Do the decrease on re-impacts also. Mobs contact damage don't have this fallof so there is no problem of player units standing on top of mobs.
	float effectMultiplier = Math::max(
		t->minPayloadsValuesMultiplier,
		1.0f - t->payloadPercentDecreasePerTargetImpact * targetImpactsCount
	);

	targetImpactsCount++;

	if (!isSourceReImpactTimer) {
		// This is received from a physics contact it is a new one (the re-impactTimer already handles this).
		if ((t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::OnlyFirstContact
			|| (t->impactAcknowledgeType == rpg3D::ProjectileTemplate::ImpactAcknowledgeType::EachContact && t->reImpactOnPersistentContactCooldownS >= 0.0f)))
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

	for (int i = 0; i < payloadsList.count(); i++) {
		std::shared_ptr<rpg3D::EffectPayload>& payloadEntry = payloadsList.getReference(i);

		payloadEntry->transferToEntity(entity, otherEntity, effectMultiplier);
	}

	if (t->maxImpactsCount > 0 && targetImpactsCount >= t->maxImpactsCount) {
		e->dispatchEvent(*EVENT_projectile_purposeFinished->setEntityData(e));

		e->getHandler()->markEntityForRemoval(e, ENT_REM_REASON_PURPOSE_FINISHED, false);
	}

	return impactedEntityInfo;
}

void PMBase::onEnvironmentContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact) {
	if (!isContact) {
		//void
	} else {
		environmentImpactsCount++;

		if (t->freezeMovementOnEnvironmentImpact) {
			physics3DModule->setDamping(1.0f, 1.0f);
			physics3DModule->clearAllForces();
			physics3DModule->setIsKinematicallyFrozen(true);
		}

		if (t->disablePayloadImpactsOnEnvironmentImpact) {
			// Unregister the activeTargetUnitFilter if active.
			if (activeTargetUnitFilter != nullptr) {
				physics3DModule->removeEntityContactFilter(activeTargetUnitFilter);

				activeTargetUnitFilter = nullptr;
			}
		}

		if (t->maxEnvironmentImpactsCount > 0 && environmentImpactsCount >= t->maxEnvironmentImpactsCount) {
			e->dispatchEvent(*EVENT_projectile_purposeFinished->setEntityData(e));

			e->getHandler()->markEntityForRemoval(e, ENT_REM_REASON_PURPOSE_FINISHED, false);
		}
	}
}

PMBase::~PMBase() {
	//void
}
