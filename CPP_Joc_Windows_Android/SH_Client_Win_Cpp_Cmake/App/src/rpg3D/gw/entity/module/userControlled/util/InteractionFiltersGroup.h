#pragma once

#include <base/list/ArrayList.h>
#include <base/event/listener/IEventListener.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include "WorldOrEntityInteractionEvent.h"
#include <functional>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>

class IEventListener;
namespace rpg3D {
	class IWorldInteractionFilter;
};

namespace rpg3D {
class InteractionFiltersGroup {pub dCtor(InteractionFiltersGroup);
	pub std::function<void(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction)> cb;

	// true: This list is set using the list setter and canno't be edited.
	pub bool isFiltersListBorrowed = false;
	pub std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> filtersList = std::make_shared<ArrayList<rpg3D::WorldOrEntityInteractionFilter>>();

	pub ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

	pub base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* connectedInteractionsDispatcher = nullptr;

	pub explicit InteractionFiltersGroup(std::function<void(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction)> cb);

	pub rpg3D::WorldOrEntityInteractionFilter* addFilter(int type);

	pub void setFiltersListBorrowed(std::shared_ptr<ArrayList<rpg3D::WorldOrEntityInteractionFilter>> filtersList);
	pub void releaseFiltersListBorrowed();

	pub bool getIsConnected();

	pub void connect(base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* interactionsDispatcher);
	pub void disconnect();

	pub virtual ~InteractionFiltersGroup();
};
};
