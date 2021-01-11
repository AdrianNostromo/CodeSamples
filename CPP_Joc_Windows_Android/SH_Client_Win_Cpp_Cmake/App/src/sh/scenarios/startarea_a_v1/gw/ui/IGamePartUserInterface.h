#pragma once

#include <base/gh.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>

namespace startarea_a_v1 {
class IGamePartUserInterface : virtual public rpg3D::IGamePartUserInterface {priv typedef rpg3D::IGamePartUserInterface super;
	pub static int TYPE;
    DEC_componentBasics();

    pub ~IGamePartUserInterface() override = default;
};
};
