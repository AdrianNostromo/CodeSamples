#pragma once

#include <base/gh.h>

namespace rpg3D {
class IConstraint3D {
	pub virtual void removeConstraint() = 0;

    pub virtual ~IConstraint3D() = default;
};
};
