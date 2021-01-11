#pragma once

#include <string>
#include <worldGame3D/gw/config/entity/EntityTags.h>

namespace rpg3D {
class EntityTags : public base::EntityTags {
public:
    static std::string ZONE_EDITOR_NOT_DELETABLE;

    // This tag is special for some scenarios, make sure it is not used for other entities.
    static std::string PLAYER_UNIT;
    static std::string UNIT;
    static std::string TREE;
    static std::string FLOWER;
    static std::string PLATFORM;
    static std::string TELEPORT;
    static std::string STRUCTURE;
    static std::string FLORA;
    static std::string WOOD;
    static std::string ROCK;
    static std::string AGGREGATE;
    static std::string DOODAD;
    static std::string PROJECTILE;
    static std::string ITEM;
    static std::string GROUND_PATCH;
    static std::string MOB;
    static std::string CREATURE;
    static std::string DUMMY;
    static std::string TURRET;
    static std::string PLACEHOLDER;
    static std::string TEXT;
    static std::string COIN;

	// Special tags.
    static std::string EDITABLE_CREATURE;
    static std::string EDITABLE_PLAYR_UNIT_DUMMY;

};
};
