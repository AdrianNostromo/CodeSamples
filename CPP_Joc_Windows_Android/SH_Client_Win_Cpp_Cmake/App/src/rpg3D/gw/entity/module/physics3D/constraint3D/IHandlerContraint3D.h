#pragma once

#include <base/gh.h>

namespace rpg3D {
	class Constraint3D;

class IHandlerContraint3D {
	pub virtual void removeConstraint(Constraint3D* constraint) = 0;

    pub virtual ~IHandlerContraint3D() = default;
};
};
