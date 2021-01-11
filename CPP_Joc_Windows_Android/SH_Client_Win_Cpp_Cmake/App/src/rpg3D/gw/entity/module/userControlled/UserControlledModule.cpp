#include "UserControlledModule.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/userEntitiesController/IUserInputDispatcher.h>
#include <rpg3D/gw/entity/module/userControlled/util/ManagedFilersList.h>

using namespace rpg3D;

UserControlledModule::UserControlledModule(
    IWorldEntity* entity,
    UserControlledTemplate* t)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

void UserControlledModule::setUserInputDispatcher(rpg3D::IUserInputDispatcher* userInputDispatcher) {
    if (this->userInputDispatcher != nullptr) {
        // Disconnect all filters.
        for (int i = 0; i < managedFiltersListsList.count(); i++) {
            rpg3D::ManagedFilersList* filter = managedFiltersListsList.getDirect(i);

            filter->disconnect();
        }

        this->userInputDispatcher = nullptr;
    }

    this->userInputDispatcher = userInputDispatcher;
    if (this->userInputDispatcher != nullptr) {
        // Connect all filters.
        for (int i = 0; i < managedFiltersListsList.count(); i++) {
            rpg3D::ManagedFilersList* filter = managedFiltersListsList.getDirect(i);

            filter->connect(userInputDispatcher);
        }
    }
}

void UserControlledModule::addManagedFiltersList(rpg3D::ManagedFilersList* managedFiltersList) {
    if (managedFiltersListsList.contains(managedFiltersList)) {
        // Already in the list.

        throw LogicException(LOC);
    }

    managedFiltersListsList.appendDirect(managedFiltersList);

    if (userInputDispatcher != nullptr) {
        managedFiltersList->connect(userInputDispatcher);
    }
}

void UserControlledModule::removeManagedFiltersListIfExists(rpg3D::ManagedFilersList* managedFiltersList) {
    int index = managedFiltersListsList.indexOf(managedFiltersList);

    if (index < 0) {
        // Not found in list.

        return;
    }

    if (userInputDispatcher != nullptr) {
        managedFiltersList->disconnect();
    }

    managedFiltersListsList.remove(index);
}

base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* UserControlledModule::getInteractionsDispatcher() {
	return &interactionsDispatcher;
}

UserControlledModule::~UserControlledModule() {
	//void
}
