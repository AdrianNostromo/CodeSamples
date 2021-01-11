#pragma once

#include <base/gh.h>
#include <rpg3D/gw/projectile/ProjectileConfig.h>

namespace rpg3D {
class ProjectileConfigsList {
	pub static bool IsStaticsInitHooked;

	pub static rpg3D::ProjectileConfig* Arrow_Basic_A_V1;
	pub static rpg3D::ProjectileConfig* Arrow_Basic_Grid06Range3Tiles_A_V1;
	pub static rpg3D::ProjectileConfig* Bullet_Basic_A_V1;
	pub static rpg3D::ProjectileConfig* Bolt_Basic_A_V1;
	pub static rpg3D::ProjectileConfig* LaserSegment_Basic_A_V1;

	pub static ProjectileConfig* newEntry(std::string id);

	priv static void InitStatics();

};
};
