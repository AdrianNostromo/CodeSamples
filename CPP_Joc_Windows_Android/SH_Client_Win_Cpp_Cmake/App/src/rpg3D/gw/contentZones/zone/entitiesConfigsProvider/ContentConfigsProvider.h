#pragma once

#include <base/gh.h>
#include <base/memory/SharedPointer.h>
#include <base/map/Map1D.h>
#include "util/EntityConfigBase.h"
#include <base/list/ArrayList.h>

namespace rpg3D {
class ContentConfigsProvider {pub dCtor(ContentConfigsProvider);
    priv Map1D<std::shared_ptr<std::string>/*instanceId*/, sp<EntityConfigBase>> configsMap{};
    
    // These are used as default entries for ground_tiledXY, tileWalls_tiledXY, posRot_for_all_other_entities;
    pub ArrayList<sp<EntityConfigBase>> fallbackEntries{};

    pub explicit ContentConfigsProvider();

    pub EntityConfigBase* getConfig(std::shared_ptr<std::string> instanceId, int configType);

    pub template <typename T>
    T getConfigAs(std::shared_ptr<std::string> instanceId, int configType);

    pub virtual ~ContentConfigsProvider();
};

template <typename T>
T ContentConfigsProvider::getConfigAs(std::shared_ptr<std::string> instanceId, int configType) {
    EntityConfigBase* entityConfig = getConfig(instanceId, configType);
    if (entityConfig == nullptr) {
        return nullptr;
    }

    return entityConfig->castAs<T>(configType);
}

};
