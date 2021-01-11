#include "PA_Base.h"
#include <rpg3D/gw/physics3D/util/WrappedPhysicsActor.h>
#include <rpg3D/gw/entity/module/physics3D/util/PhysicsEntityCallbackFilter.h>

using namespace rpg3D;

PA_Base::PA_Base(IWorldEntity* e, int physicsGroupsBitMap, int anyInteraction_maskBitMap, int hardImpact_maskBitMap)
	: super(),
	e(e),
	physicsGroupsBitMap(physicsGroupsBitMap), anyInteraction_maskBitMap(anyInteraction_maskBitMap), hardImpact_maskBitMap(hardImpact_maskBitMap)
{
	wrapper = std::make_shared<rpg3D::WrappedPhysicsActor>(this);
}

IWorldEntity* PA_Base::getEntity() {
	return e;
}

int PA_Base::getPhysicsGroupsBitMap() {
	return physicsGroupsBitMap;
}

int PA_Base::getHardImpact_maskBitMap() {
	return hardImpact_maskBitMap;
}

std::shared_ptr<rpg3D::WrappedPhysicsActor> PA_Base::getWrapper() {
	if (wrapper == nullptr || wrapper->physicsActor == nullptr) {
		// Logic error.
		throw LogicException(LOC);
	}

	return wrapper;
}

IPhysicsEntityCallbackFilter* PA_Base::addEntityContactFilter(std::function<void(
	IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact)> cb_onContact)
{
	std::shared_ptr<PhysicsEntityCallbackFilter> filter{ new PhysicsEntityCallbackFilter(this, cb_onContact) };

	contactFilters.appendReference(filter);

	return filter.get();
}

void PA_Base::removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) {
	for (int i = 0; i < contactFilters.count(); i++) {
		std::shared_ptr<PhysicsEntityCallbackFilter> cFilter = contactFilters.getDirect(i);

		if (cFilter.get() == localFilter) {
			cFilter->silentDisconnectAllContacts();
			if (cFilter->getActiveContactsCount() > 0) {
				throw LogicException(LOC);
			}

			contactFilters.removeReference(cFilter);

			return;
		}
	}

	throw LogicException(LOC);
}

int& PA_Base::getUsedInChangeContactsCount() {
	return usedInChangeContactsCount;
}

void PA_Base::reservedDisposePre() {
	if (isDisposedPre) {
		throw LogicException(LOC);
	}

	disposePre();

	isDisposedPre = true;
}

void PA_Base::disposePre() {
	if (usedInChangeContactsCount > 0) {
		// This pActor is used in a unprocessed contact_change.
		/// An error will occur after this entity is removed because the contact_change has a wrapper for this pActor and when this pActor is disposed it will have a nullptr pActor which is not allowed with the current logic (for error detection).
		/// This may occur if a pBody is remove from the pWorld while it has the pActor active and on the same frame, the entity is disposed.
		throw LogicException(LOC);
	}

	//void
}

void PA_Base::disposeMain() {
	if (!isDisposedPre) {
		// Must call reservedDisposePre first.
		throw LogicException(LOC);
	}

	if (usedInChangeContactsCount > 0) {
		// This pActor is used in a unprocessed contact_change.
		/// An error will occur after this entity is removed because the contact_change has a wrapper for this pActor and when this pActor is disposed it will have a nullptr pActor which is not allowed with the current logic (for error detection).
		/// This may occur if a pBody is remove from the pWorld while it has the pActor active and on the same frame, the entity is disposed.
		throw LogicException(LOC);
	}

	if (wrapper->physicsActor != nullptr) {
		// Also do this here because entities may be manually removed (eg: on area change).
		wrapper->physicsActor = nullptr;
	}

	super::disposeMain();
}

PA_Base::~PA_Base() {
	//void
}
