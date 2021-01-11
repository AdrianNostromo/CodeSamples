#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include "IUserControlledModule.h"
#include <base/list/ArrayList.h>

namespace rpg3D {
class UserControlledModule : public EntityModule, virtual public IUserControlledModule {priv typedef EntityModule super;pub dCtor(UserControlledModule);
	pub UserControlledTemplate* t;

	priv ArrayList<rpg3D::ManagedFilersList*> managedFiltersListsList{};

	priv rpg3D::IUserInputDispatcher* userInputDispatcher = nullptr;
	
	priv base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>> interactionsDispatcher{};

    pub explicit UserControlledModule(
	    IWorldEntity* entity,
	    UserControlledTemplate* t);

	pub void setUserInputDispatcher(rpg3D::IUserInputDispatcher* userInputDispatcher) final;

	pub void addManagedFiltersList(rpg3D::ManagedFilersList* managedFiltersList) final;
	pub void removeManagedFiltersListIfExists(rpg3D::ManagedFilersList* managedFiltersList) final;

	pub base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* getInteractionsDispatcher() override;

	pub ~UserControlledModule() override;
};
};
