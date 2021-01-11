#include "ManagedFilersList.h"
#include <rpg3D/gw/userEntitiesController/IUserInputDispatcher.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

ManagedFilersList::ManagedFilersList() {
	//void
}

void ManagedFilersList::connect(rpg3D::IUserInputDispatcher* userInputDispatcher) {
	if (this->userInputDispatcher != nullptr || userInputDispatcher == nullptr) {
		// Must set to nullptr before setting to a different userInputDispatcher.
		throw LogicException(LOC);
	}

	this->userInputDispatcher = userInputDispatcher;

	for (int i = 0; i < customFiltersList.count(); i++) {
		std::shared_ptr<rpg3D::IWorldInteractionFilter> filter = customFiltersList.getDirect(i);

		userInputDispatcher->hookCustomFilter(filter.get());
	}
}

void ManagedFilersList::disconnect() {
	if (userInputDispatcher == nullptr) {
		// Not connected.
		throw LogicException(LOC);
	}

	for (int i = 0; i < customFiltersList.count(); i++) {
		std::shared_ptr<rpg3D::IWorldInteractionFilter> filter = customFiltersList.getDirect(i);

		userInputDispatcher->releaseCustomFilter(filter.get());
	}

	this->userInputDispatcher = nullptr;
}

void ManagedFilersList::addFilter(std::shared_ptr<rpg3D::IWorldInteractionFilter> filter) {
	if (customFiltersList.contains(filter)) {
		throw LogicException(LOC);
	}

	customFiltersList.appendDirect(filter);

	if (userInputDispatcher != nullptr) {
		userInputDispatcher->hookCustomFilter(filter.get());
	}
}

void ManagedFilersList::removeFilter(std::shared_ptr<rpg3D::IWorldInteractionFilter> filter) {
	int index = customFiltersList.indexOf(filter);
	if (index < 0) {
		throw LogicException(LOC);
	}

	if (userInputDispatcher != nullptr) {
		userInputDispatcher->releaseCustomFilter(filter.get());
	}

	customFiltersList.remove(index);
}

ManagedFilersList::~ManagedFilersList() {
	//void
}
