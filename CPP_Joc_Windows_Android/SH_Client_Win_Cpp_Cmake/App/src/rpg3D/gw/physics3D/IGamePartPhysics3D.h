#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <string>

class btDiscreteDynamicsWorld;

namespace rpg3D {
class IGamePartPhysics3D : virtual public IGamePart {
	priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	// This is used as the id for the runtime setting for the physics 3d debug.
	pub static std::string RUNTIME_SETTING_physics3DDebugRender;
	pub static std::string RUNTIME_SETTING_physics3DDebugClearDepthBuffer;

	pub virtual btDiscreteDynamicsWorld* getDynamicsWorld() = 0;

	pub virtual void setIsGroundPlaneActive(bool isGroundPlaneActive) = 0;

    pub ~IGamePartPhysics3D() override = default;
};
};
