#pragma once

#include <base/gh.h>
#include "CCD_Base.h"
#include <base/list/ListDL.h>
#include <memory>
#include <base/MM.h>

namespace rpg3D {
	class WrappedPhysicsActor;

class CCD_ChangeDetector : public CCD_Base {priv typedef CCD_Base super;pub dCtor(CCD_ChangeDetector);
	pub class ChangeEntry {pub dCtor(ChangeEntry);
		pub std::shared_ptr<rpg3D::WrappedPhysicsActor> wPActorA;
		pub std::shared_ptr<rpg3D::WrappedPhysicsActor> wPActorB;

		pub bool isContact;

		pub explicit ChangeEntry(
			std::shared_ptr<rpg3D::WrappedPhysicsActor> wPActorA, std::shared_ptr<rpg3D::WrappedPhysicsActor> wPActorB,
			bool isContact)
			: wPActorA(wPActorA), wPActorB(wPActorB),
			isContact(isContact)
		{
			//void
		}
	};

	// If this is negative, an error occured.
	priv int totalFullContactsCount = 0;
	
	pub ListDL<ChangeEntry> changesList{LOC};

	pub explicit CCD_ChangeDetector(btCollisionConfiguration* collisionConfiguration);

	pub int getTotalFullContactsCount();

	pub void onManifoldFullConnection(CustomPersistentManifold* manifold, bool isFullContact) final;

	pub ~CCD_ChangeDetector() override;
};
};
