#pragma once

#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace battlezones_a_v1 {
class EntityIds : public rpg3D::EntityIds {
public:
    static std::shared_ptr<std::string> defaultSpawnLocation;
    static std::shared_ptr<std::string> platform_Teleporter_Home;

    static std::shared_ptr<std::string> platform_zonesGroupStart;

    static std::shared_ptr<std::string> platform_zoneTeleporter_next;
    static std::shared_ptr<std::string> platform_zoneTeleporter_previous;

};
};
