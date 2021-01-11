#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class T_TownTeleporter_Wood {
	pub static EntityTemplate* getOrCreateTemplate();

	priv static Drawable3DCreateConfig_VoxelGrid* buildBase();

};
};
};
