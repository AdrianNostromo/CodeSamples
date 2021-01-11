#include "PhysicsActor.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/module/physics3D/util/PhysicsEntityCallbackFilter.h>

using namespace rpg3D;

PhysicsActor::PhysicsActor(IWorldEntity* e, int physicsGroupsBitMap, int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	: super(e, physicsGroupsBitMap, anyInteraction_maskBitMap, hardImpact_maskBitMap)
{
	//void
}

std::shared_ptr<PhysicsActor::RemoteContact> PhysicsActor::getRemoteContactEntry(IPhysicsEntityCallbackFilter* remoteFilter, bool createIfNotExists) {
	for (int i = 0; i < remoteFilterContactsList.count(); i++) {
		std::shared_ptr<RemoteContact> remoteContact = remoteFilterContactsList.getDirect(i);

		if (remoteContact->remoteFilter == remoteFilter) {
			return remoteContact;
		}
	}

	if (createIfNotExists) {
		return *remoteFilterContactsList.appendDirect(std::make_shared<RemoteContact>(remoteFilter));
	}

	return nullptr;
}

void PhysicsActor::pushRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) {
	std::shared_ptr<RemoteContact> remoteContact = getRemoteContactEntry(remoteFilter, true);

	remoteContact->contactsCount++;
}

void PhysicsActor::removeRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) {
	std::shared_ptr<RemoteContact> remoteContact = getRemoteContactEntry(remoteFilter, false);
	if (remoteContact == nullptr || remoteContact->contactsCount <= 0) {
		throw LogicException(LOC);
	}
	remoteContact->contactsCount--;

	if (remoteContact->contactsCount == 0) {
		// Remove the entry completly.
		if (!remoteFilterContactsList.removeDirect(remoteContact)) {
			// Entry not found error.
			throw LogicException(LOC);
		}
	}
}

void PhysicsActor::removeAllRemoteFilterContacts(IPhysicsEntityCallbackFilter* remoteFilter, int validationTotalContactsCount) {
	std::shared_ptr<RemoteContact> remoteContact = getRemoteContactEntry(remoteFilter, false);
	if (remoteContact == nullptr || remoteContact->contactsCount != validationTotalContactsCount) {
		throw LogicException(LOC);
	}

	remoteContact->contactsCount = 0;
	// Remove the entry completly.
	if (!remoteFilterContactsList.removeDirect(remoteContact)) {
		// Entry not found error.
		throw LogicException(LOC);
	}
}

void PhysicsActor::onContactChanged(IPhysicsActor* otherEntity, bool isContact) {
	// Check each filter. On filter pass, save in the filter contacts list.
	for (int i = 0; i < contactFilters.count(); i++) {
		std::shared_ptr<PhysicsEntityCallbackFilter> cFilter = contactFilters.getDirect(i);

		cFilter->onContactChanged(
			this, e,
			otherEntity,
			isContact
		);
	}
}

void PhysicsActor::disconnectAllContacts() {
	// Disconnect all remote_filter contacts.
	while (remoteFilterContactsList.count() > 0) {
		std::shared_ptr<RemoteContact> remoteContact = remoteFilterContactsList.getDirect(0);

		// This call will remove the entry from remoteFilterContactsList when it is called.
		remoteContact->remoteFilter->disconnectAllPActorContacts(this);
	}

	// Disconnect all filters.
	for (int i = 0; i < contactFilters.count(); i++) {
		std::shared_ptr<PhysicsEntityCallbackFilter> filter = contactFilters.getDirect(i);

		filter->silentDisconnectAllContacts();
	}
}

void PhysicsActor::disposePre() {
	disconnectAllContacts();

	super::disposePre();
}

void PhysicsActor::disposeMain() {
	//void

	super::disposeMain();
}

PhysicsActor::~PhysicsActor() {
	//void
}
