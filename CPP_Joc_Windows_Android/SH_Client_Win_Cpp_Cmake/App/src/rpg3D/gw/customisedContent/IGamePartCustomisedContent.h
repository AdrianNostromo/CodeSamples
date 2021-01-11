#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

class btDiscreteDynamicsWorld;
namespace rpg3D {
	class ICustomisedContentHandler;
};

namespace rpg3D {
class IGamePartCustomisedContent : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual rpg3D::ICustomisedContentHandler* getCustomisedContentHandler() = 0;
	pub virtual void setCustomisedContentHandler(rpg3D::ICustomisedContentHandler* customisedContentHandler) = 0;

    pub ~IGamePartCustomisedContent() override = default;
};
};
