#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace base {
	class ManagedFlags;
};

namespace rpg3D {
class IOGPZoneFlags : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual base::ManagedFlags* getZoneFlags() = 0;

    pub ~IOGPZoneFlags() override = default;
};
};
