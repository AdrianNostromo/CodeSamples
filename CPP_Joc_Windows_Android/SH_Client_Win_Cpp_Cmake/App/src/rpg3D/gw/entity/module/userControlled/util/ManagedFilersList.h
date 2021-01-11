#pragma once

#include <base/gh.h>
#include <memory>
#include <base/list/ArrayList.h>

namespace rpg3D {
    class IUserInputDispatcher;
    class IWorldInteractionFilter;
};

namespace rpg3D {
class ManagedFilersList {pub dCtor(ManagedFilersList);
    priv ArrayList<std::shared_ptr<rpg3D::IWorldInteractionFilter>> customFiltersList{};
    
    priv rpg3D::IUserInputDispatcher* userInputDispatcher = nullptr;

    pub explicit ManagedFilersList();

    pub void connect(rpg3D::IUserInputDispatcher* userInputDispatcher);
    pub void disconnect();

    pub void addFilter(std::shared_ptr<rpg3D::IWorldInteractionFilter> filter);
    pub void removeFilter(std::shared_ptr<rpg3D::IWorldInteractionFilter> filter);

    pub virtual ~ManagedFilersList();
};
};
