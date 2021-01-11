#pragma once

#include <base/gh.h>
#include <memory>
#include <functional>

class IWorldEntity;

namespace rpg3D {
	class WrappedPhysicsActor;
	class IPhysicsEntityCallbackFilter;

class IPhysicsActor {
	pub virtual std::shared_ptr<rpg3D::WrappedPhysicsActor> getWrapper() = 0;

	pub virtual IWorldEntity* getEntity() = 0;

	pub virtual int getPhysicsGroupsBitMap() = 0;
	pub virtual int getHardImpact_maskBitMap() = 0;

	pub virtual void pushRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) = 0;
	pub virtual void removeRemoteFilterContact(IPhysicsEntityCallbackFilter* remoteFilter) = 0;
	pub virtual void removeAllRemoteFilterContacts(IPhysicsEntityCallbackFilter* remoteFilter, int validationTotalContactsCount) = 0;
	
	pub virtual void onContactChanged(IPhysicsActor* otherEntity, bool isContact) = 0;

	pub virtual IPhysicsEntityCallbackFilter* addEntityContactFilter(std::function<void(
		IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact)> cb_onContact) = 0;
	pub virtual void removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) = 0;

	pub virtual void disconnectAllContacts() = 0;

	pub virtual int& getUsedInChangeContactsCount() = 0;

    pub virtual ~IPhysicsActor() = default;
};
};
