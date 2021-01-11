#pragma once

#include <base/gh.h>
#include <string>
#include <rpg3D/gw/cache/CacheIds.h>

namespace startarea_a_v1 {
class CacheIds : public rpg3D::CacheIds {priv typedef rpg3D::CacheIds super;
    pub static std::string persistentEntititesList;

    pub static std::string persistentEntity_prefix;
    pub static std::string persistentEntity_sufix;

};
};
