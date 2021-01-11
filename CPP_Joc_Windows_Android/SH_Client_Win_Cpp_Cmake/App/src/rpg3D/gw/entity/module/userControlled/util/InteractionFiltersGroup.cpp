#include "InteractionFiltersGroup.h"

using namespace rpg3D;

InteractionFiltersGroup::InteractionFiltersGroup(std::function<void(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction)> cb)
	: cb(cb)
{
	//void
}

rpg3D::WorldOrEntityInteractionFilter* InteractionFiltersGroup::addFilter(int type) {
	if(isFiltersListBorrowed) {
		// The list is not managed by this. It was set using the list setter.
		throw LogicException(LOC);
	}

	return filtersList->append_emplace(type);
}

void InteractionFiltersGroup::setFiltersListBorrowed(std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> filtersList) {
	isFiltersListBorrowed = true;

	this->filtersList = filtersList;
}

void InteractionFiltersGroup::releaseFiltersListBorrowed() {
	filtersList = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();

	isFiltersListBorrowed = false;
}

bool InteractionFiltersGroup::getIsConnected() {
	return (connectedInteractionsDispatcher != nullptr) ? true : false;
}

void InteractionFiltersGroup::connect(base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* interactionsDispatcher) {
	if(getIsConnected()) {
		if(connectedInteractionsDispatcher == interactionsDispatcher) {
			// Already connected to target dispatcher.
			return ;
		}else {
			disconnect();
		}
	}
	connectedInteractionsDispatcher = interactionsDispatcher;

	for(int i=0;i<filtersList->count();i++) {
		rpg3D::WorldOrEntityInteractionFilter* filter = filtersList->getPointer(i);

		rpg3D::IWorldOrEntityInteractionFilter* cFilter = filter;
		interactionsDispatcher->getDispatcher(filter->getType(), true)->addListener(cb, &autoListenersList, nullptr/*cb_onHandlerDisconnected*/, cFilter/*extraData*/);
	}
}

void InteractionFiltersGroup::disconnect() {
	if(connectedInteractionsDispatcher == nullptr) {
		return ;
	}
	connectedInteractionsDispatcher = nullptr;

	autoListenersList.clear();

	//asd_tool_1;
}

InteractionFiltersGroup::~InteractionFiltersGroup() {
	//void
}
