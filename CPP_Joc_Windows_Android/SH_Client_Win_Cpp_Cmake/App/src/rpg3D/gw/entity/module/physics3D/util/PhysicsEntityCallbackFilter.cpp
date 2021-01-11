#include "PhysicsEntityCallbackFilter.h"
#include <rpg3D/gw/physics3D/actor/IPhysicsActor.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

PhysicsEntityCallbackFilter::PhysicsEntityCallbackFilter(
	IPhysicsActor* pActor,
	std::function<void(IPhysicsEntityCallbackFilter * filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact)
	: super(),
	pActor(pActor),
	cb_onContact(cb_onContact)
{
	//void
}

std::shared_ptr<PhysicsEntityCallbackFilter::ActiveContact> PhysicsEntityCallbackFilter::getActiveContactEntry(IPhysicsActor* contactedPActor, bool createIfNotExists) {
	for (int i = 0; i < activeContacts.count(); i++) {
		std::shared_ptr<ActiveContact> activeContact = activeContacts.getDirect(i);

		if (activeContact->contactedPActor == contactedPActor) {
			return activeContact;
		}
	}

	if (createIfNotExists) {
		return *activeContacts.appendDirect(std::make_shared<ActiveContact>(contactedPActor));
	}

	return nullptr;
}

IEntityFilter* PhysicsEntityCallbackFilter::setCollissionMaskBitMapOneWay(int collissionMaskBitMapOneWay) {
	this->collissionMaskBitMapOneWay = collissionMaskBitMapOneWay;

	return this;
}

int PhysicsEntityCallbackFilter::getActiveContactsCount() {
	return activeContacts.count();
}

void PhysicsEntityCallbackFilter::disconnectAllPActorContacts(IPhysicsActor* otherPhysicsActor) {
	std::shared_ptr<ActiveContact> activeContact = getActiveContactEntry(otherPhysicsActor, false);
	if (activeContact == nullptr || activeContact->contactsCount <= 0) {
		throw LogicException(LOC);
	}

	// This must call the cb for each contactsCount.
	while (activeContact->contactsCount) {
		onContactChanged(
			pActor, pActor->getEntity(),
			otherPhysicsActor,
			false
		);
	}
}

void PhysicsEntityCallbackFilter::silentDisconnectAllContacts() {
	while(activeContacts.count() > 0) {
		std::shared_ptr<ActiveContact> activeContact = activeContacts.removeAndGetDirect(0);

		activeContact->contactedPActor->removeAllRemoteFilterContacts(this, activeContact->contactsCount);
	}
}

void PhysicsEntityCallbackFilter::onContactChanged(
	IPhysicsActor* selfPhysicsActor, IWorldEntity* selfEntity,
	IPhysicsActor* otherPhysicsActor,
	bool isContact)
{
	if (isContact) {
		if (testCollissionFlagsOneWay(collissionMaskBitMapOneWay, otherPhysicsActor)
			&& testEntity(selfEntity, otherPhysicsActor->getEntity()))
		{
			std::shared_ptr<ActiveContact> activeContact = getActiveContactEntry(otherPhysicsActor, true);

			activeContact->contactsCount++;
			otherPhysicsActor->pushRemoteFilterContact(this);

			if (activeContact->contactsCount == 1) {
				if (cb_onContact) {
					cb_onContact(this, otherPhysicsActor->getEntity(), true);
				}
			}
		}
	} else {
		std::shared_ptr<ActiveContact> activeContact = getActiveContactEntry(otherPhysicsActor, false);
		// activeContact is nullptr if the contact_on test was false (normal behavious).

		if (activeContact != nullptr) {
			activeContact->contactsCount--;
			otherPhysicsActor->removeRemoteFilterContact(this);

			if (activeContact->contactsCount == 0) {
				if (!activeContacts.removeDirect(activeContact)) {
					throw LogicException(LOC);
				}

				if (cb_onContact) {
					cb_onContact(this, otherPhysicsActor->getEntity(), false);
				}
			}
		}
	}
}

bool PhysicsEntityCallbackFilter::testCollissionFlagsOneWay(int collissionMaskBitMapOneWay, IPhysicsActor* otherPhysicsActor) {
	if ((otherPhysicsActor->getPhysicsGroupsBitMap() & collissionMaskBitMapOneWay) != 0) {
		return true;
	}

	return false;
}

PhysicsEntityCallbackFilter::~PhysicsEntityCallbackFilter() {
	//void
}
