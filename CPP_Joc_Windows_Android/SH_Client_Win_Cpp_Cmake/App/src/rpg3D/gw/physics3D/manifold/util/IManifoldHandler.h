#pragma once

#include <base/gh.h>

namespace rpg3D {
class CustomPersistentManifold;
class IManifoldHandler {
	pub virtual void onManifoldFullConnection(CustomPersistentManifold* manifold, bool isFullContact) = 0;

    pub virtual ~IManifoldHandler() = default;
};
};
