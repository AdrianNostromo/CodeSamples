#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <base/list/ArrayList.h>
#include <base/event/listener/IEventListener.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

namespace rpg3D {
	class IUserInputDispatcher;
	class ManagedFilersList;
};

namespace rpg3D {
class IUserControlledModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* getInteractionsDispatcher() = 0;

	pub virtual void setUserInputDispatcher(rpg3D::IUserInputDispatcher* userInputDispatcher) = 0;

	pub virtual void addManagedFiltersList(rpg3D::ManagedFilersList* managedFiltersList) = 0;
	pub virtual void removeManagedFiltersListIfExists(rpg3D::ManagedFilersList* managedFiltersList) = 0;

	pub ~IUserControlledModule() override = default;
};
};
