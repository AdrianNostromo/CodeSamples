#pragma once

#include <base/gh.h>
#include "E3D_Camera.h"

namespace graphics {
	class CinematicController;
};

namespace graphics {
class E3D_Cinematic : public E3D_Camera {priv typedef E3D_Camera super;pub dCtor(E3D_Cinematic);
	priv static const bool IS_DEBUG = false;

	// The last one in the list.
	priv std::shared_ptr<CinematicController> activeCinematicController = nullptr;
	
	pub explicit E3D_Cinematic();

	pub std::shared_ptr<ICinematicController> hookCinematicController(std::string location);
	pub int releaseCinematicController(std::shared_ptr<ICinematicController> target) override;

    pub ~E3D_Cinematic() override;
};
};
