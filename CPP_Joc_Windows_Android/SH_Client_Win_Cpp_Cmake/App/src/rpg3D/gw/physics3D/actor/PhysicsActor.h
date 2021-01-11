#pragma once

#include <base/gh.h>
#include "PA_Base.h"

class btRigidBody;
namespace rpg3D {
	class PhysicsEntityCallbackFilter;
};

// This is used for custom physics objects (eg. swords).
namespace rpg3D {
class PhysicsActor : public PA_Base {priv typedef PA_Base super; pub dCtor(PhysicsActor);
	priv class RemoteContact {
		// Can use a PhysicsEntityCallbackFilter* because the connections are removed before the remove filter is deleted.
		pub IPhysicsEntityCallbackFilter* remoteFilter;
		pub int contactsCount = 0;

		pub explicit RemoteContact(IPhysicsEntityCallbackFilter* remoteFilter)
			: remoteFilter(remoteFilter)
		{
			//void
		}
	};

	// These are set on the other_pActor when a filter is activated. The local filter will have a contact to the other_actor.
	// This list is used to remove contacts when a entity is disposed.
	// This may contain duplicates if multiple filters are activated on a remote_actor.
	// Bullet_physics may cause multiple contacts for the same pBodes and all must be handled by the filter so this list may contain duplicates. In that case use a counter and a bulk remove function on this entity disposal.
	priv ArrayList<std::shared_ptr<RemoteContact>> remoteFilterContactsList{};

    pub explicit PhysicsActor(IWorldEntity* e, int physicsGroupsBitMap, int anyInteraction_maskBitMap, int hardImpact_maskBitMap);

	priv std::shared_ptr<RemoteContact> getRemoteContactEntry(IPhysicsEntityCallbackFilter* remoteFilter, bool createIfNotExists);

	pub void pushRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) final;
	pub void removeRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) final;
	pub void removeAllRemoteFilterContacts(IPhysicsEntityCallbackFilter* remoteFilter, int validationTotalContactsCount) final;

	pub void onContactChanged(IPhysicsActor* otherEntity, bool isContact) final;

	// Note. When calling this before a pWorld.removeRigidBody(...), also remove this pActor from the pBody userData before it is removed from the pWorld.
	/// If this is not done and the pBody is disposed after it is removed from the pWorld, there will be a physics contact break detected between 2 pActors but this pActor gets disposed. That will cause a logic error (Once pActors contact or contact_break is saved in the list, it must remain valid until it gets processed after the physics sim).
	pub void disconnectAllContacts() final;

	prot void disposePre() override;
	prot void disposeMain() override;
    pub ~PhysicsActor() override;
};
};
