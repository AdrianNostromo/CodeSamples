#include "E3D_Cinematic.h"
#include <graphics/environment/environment3D/cinematic/CinematicController.h>
#include <iostream>
#include <base/app/config/AppConfig.h>
#include <assert.h>

using namespace graphics;

E3D_Cinematic::E3D_Cinematic()
	: super()
{
	//void
}

std::shared_ptr<ICinematicController> E3D_Cinematic::hookCinematicController(std::string location) {
	if (AppConfig::IS_DEBUG && IS_DEBUG) {
		std::cout << "Hooking a new game world cinematic controller with location: " << location << std::endl;
	}

	std::shared_ptr<CinematicController> cc = std::make_shared<CinematicController>(location, this);
	if (activeCinematicController) {
		if (activeCinematicController->post) {
			return nullptr;
		}

		activeCinematicController->post = cc;
		cc->prev = activeCinematicController;

		if (activeCinematicController->setIsActive(false) != 0) {
			return nullptr;
		}
	}

	activeCinematicController = cc;
	if (cc->setIsActive(true) != 0) {
		return nullptr;
	}

	return static_cast<std::shared_ptr<ICinematicController>>(cc);
}

int E3D_Cinematic::releaseCinematicController(std::shared_ptr<ICinematicController> _target) {
	std::shared_ptr<CinematicController> target = std::dynamic_pointer_cast<CinematicController>(_target);

	std::cout << "Releasing a game world cinematic controller with location: " << target->getLocation() << std::endl;

	if (target->prev) {
		target->prev->post = target->post;
	}
	if (target->post) {
		target->post->prev = target->prev;
	}

	if (activeCinematicController == target) {
		if (activeCinematicController->setIsActive(false) != 0) {
			return -1;
		}

		activeCinematicController = activeCinematicController->prev;
		if (activeCinematicController) {
			if (activeCinematicController->setIsActive(true) != 0) {
				return -2;
			}
		}
	}

	target->prev = nullptr;
	target->post = nullptr;

	return 0;
}

E3D_Cinematic::~E3D_Cinematic() {
	// The controllers must have been released already.
	assert(!activeCinematicController);
}
