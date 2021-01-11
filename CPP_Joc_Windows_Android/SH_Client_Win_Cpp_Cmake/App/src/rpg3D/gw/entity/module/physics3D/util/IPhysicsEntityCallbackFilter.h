#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/filter/IEntityFilter.h>

namespace rpg3D {
	class IPhysicsActor;

class IPhysicsEntityCallbackFilter : virtual public IEntityFilter {
	pub virtual IEntityFilter* setCollissionMaskBitMapOneWay(int collissionMaskBitMapOneWay) = 0;

	pub virtual int getActiveContactsCount() = 0;
	pub virtual void disconnectAllPActorContacts(IPhysicsActor* otherPhysicsActor) = 0;

	pub virtual void onContactChanged(
		IPhysicsActor* selfPhysicsActor, IWorldEntity* selfEntity,
		IPhysicsActor* otherPhysicsActor,
		bool isContact) = 0;

    pub ~IPhysicsEntityCallbackFilter() override = default;
};
};
