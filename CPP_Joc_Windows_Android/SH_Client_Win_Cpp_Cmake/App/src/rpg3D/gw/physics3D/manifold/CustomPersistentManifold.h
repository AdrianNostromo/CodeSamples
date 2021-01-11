#pragma once

#include <base/gh.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>

namespace rpg3D {
class IManifoldHandler;
class CustomPersistentManifold : public btPersistentManifold {priv typedef btPersistentManifold super;pub dCtor(CustomPersistentManifold);
	priv IManifoldHandler* handler;

	// true : (m_cachedPoints > 0);
	priv bool isFullContact = false;

    pub explicit CustomPersistentManifold(
		const btCollisionObject* body0, const btCollisionObject* body1, int, btScalar contactBreakingThreshold, btScalar contactProcessingThreshold,
		IManifoldHandler* handler);

	pub bool getIsFullContact();

	pub void setBodies(const btCollisionObject* body0, const btCollisionObject* body1) override;

	pub void setNumContacts(int cachedPoints) override;
	
	pub int addManifoldPoint(const btManifoldPoint& newPoint, bool isPredictive = false) override;
	pub void removeContactPoint(int index) override;

	pub SIMD_FORCE_INLINE void clearManifold() override;

	pub virtual void deSerialize(const struct btPersistentManifoldDoubleData* manifoldDataPtr) override;
	pub virtual void deSerialize(const struct btPersistentManifoldFloatData* manifoldDataPtr) override;

	priv void onContactsChange();

    pub ~CustomPersistentManifold() override;
};
};
