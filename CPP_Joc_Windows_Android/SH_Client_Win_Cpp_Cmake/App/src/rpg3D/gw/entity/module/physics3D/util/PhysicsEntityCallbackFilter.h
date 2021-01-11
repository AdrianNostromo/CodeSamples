#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/filter/EntityFilter.h>
#include "IPhysicsEntityCallbackFilter.h"
#include <functional>
#include <memory>
#include <base/list/ArrayList.h>
#include <memory>

namespace base {
	class WrappedWorldEntity;
};

namespace rpg3D {
class PhysicsEntityCallbackFilter : public EntityFilter, virtual public IPhysicsEntityCallbackFilter {priv typedef EntityFilter super;pub dCtor(PhysicsEntityCallbackFilter);
	priv class ActiveContact {
		pub IPhysicsActor* contactedPActor;

		pub int contactsCount = 0;

		pub explicit ActiveContact(IPhysicsActor* contactedPActor)
			: contactedPActor(contactedPActor)
		{
			//void
		}
	};

	priv ArrayList<std::shared_ptr<ActiveContact>> activeContacts{};

	priv IPhysicsActor* pActor;
	priv std::function<void(IPhysicsEntityCallbackFilter * filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact;

	// -1; All checks pass.
	priv int collissionMaskBitMapOneWay = -1;

    pub explicit PhysicsEntityCallbackFilter(
		IPhysicsActor* pActor,
		std::function<void(IPhysicsEntityCallbackFilter * filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact);

	priv std::shared_ptr<ActiveContact> getActiveContactEntry(IPhysicsActor* contactedPActor, bool createIfNotExists);

	pub IEntityFilter* setCollissionMaskBitMapOneWay(int collissionMaskBitMapOneWay) final;

	pub int getActiveContactsCount() final;
	pub void disconnectAllPActorContacts(IPhysicsActor* otherPhysicsActor) final;

	// No cb is called.
	pub void silentDisconnectAllContacts();

	pub void onContactChanged(
		IPhysicsActor* selfPhysicsActor, IWorldEntity* selfEntity,
		IPhysicsActor* otherPhysicsActor,
		bool isContact) final;

	priv static bool testCollissionFlagsOneWay(int collissionMaskBitMapOneWay, IPhysicsActor* otherPhysicsActor);

    pub ~PhysicsEntityCallbackFilter() override;
};
};
