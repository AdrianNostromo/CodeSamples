#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <memory>
#include <string>

namespace rpg3D {
class IGamePartContentZones : virtual public worldGame3D::IGamePartContentZones {priv typedef worldGame3D::IGamePartContentZones super;
	pub static int TYPE;
    DEC_componentBasics();

	//void

    pub ~IGamePartContentZones() override = default;
};
};
