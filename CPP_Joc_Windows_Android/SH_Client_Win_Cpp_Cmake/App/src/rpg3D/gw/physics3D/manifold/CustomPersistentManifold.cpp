#include "CustomPersistentManifold.h"
#include "util/IManifoldHandler.h"

using namespace rpg3D;

CustomPersistentManifold::CustomPersistentManifold(
	const btCollisionObject* body0, const btCollisionObject* body1, int i, btScalar contactBreakingThreshold, btScalar contactProcessingThreshold,
	IManifoldHandler* handler)
	: super(body0, body1, i, contactBreakingThreshold, contactProcessingThreshold),
	handler(handler)
{
	//void
}

bool CustomPersistentManifold::getIsFullContact() {
	return isFullContact;
}

void CustomPersistentManifold::setBodies(const btCollisionObject* body0, const btCollisionObject* body1) {
	if (isFullContact) {
		// Because bodies might change, do a completly disconnect event.
		isFullContact = false;

		handler->onManifoldFullConnection(this, false);
	}

	super::setBodies(body0, body1);

	onContactsChange();
}

void CustomPersistentManifold::setNumContacts(int cachedPoints) {
	super::setNumContacts(cachedPoints);

	onContactsChange();
}

int CustomPersistentManifold::addManifoldPoint(const btManifoldPoint& newPoint, bool isPredictive) {
	int ret = super::addManifoldPoint(newPoint, isPredictive);

	onContactsChange();
	
	return ret;
}

void CustomPersistentManifold::removeContactPoint(int index) {
	super::removeContactPoint(index);

	onContactsChange();
}

SIMD_FORCE_INLINE void CustomPersistentManifold::clearManifold() {
	super::clearManifold();

	onContactsChange();
}

void CustomPersistentManifold::deSerialize(const struct btPersistentManifoldDoubleData* manifoldDataPtr) {
	super::deSerialize(manifoldDataPtr);

	onContactsChange();
}

void CustomPersistentManifold::deSerialize(const struct btPersistentManifoldFloatData* manifoldDataPtr) {
	super::deSerialize(manifoldDataPtr);

	onContactsChange();
}

void CustomPersistentManifold::onContactsChange() {
	bool b = getNumContacts() > 0 ? true : false;
	if (isFullContact != b) {
		isFullContact = b;

		handler->onManifoldFullConnection(this, isFullContact);
	}
}

CustomPersistentManifold::~CustomPersistentManifold() {
	//void
}
