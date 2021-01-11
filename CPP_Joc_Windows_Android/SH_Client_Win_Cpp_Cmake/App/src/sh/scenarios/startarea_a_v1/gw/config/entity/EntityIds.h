#pragma once

#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace startarea_a_v1 {
class EntityIds : public rpg3D::EntityIds {
public:
    static std::shared_ptr<std::string> platformCreatureCreator;
    static std::shared_ptr<std::string> player_hero_dummy;

};
};
