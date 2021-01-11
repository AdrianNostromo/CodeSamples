#pragma once

#include <rpg3D/gw/projectile/ProjectileConfig.h>
#include <rpg3D/gw/config/projectile/ProjectileConfigsList.h>

namespace battlezones_a_v1 {
class ProjectileConfigsList : public rpg3D::ProjectileConfigsList {
public:
	static bool IsStaticsInitHooked;

public:
	static rpg3D::ProjectileConfig* newEntry(std::string id);

private:
	static void InitStatics();

};
};
