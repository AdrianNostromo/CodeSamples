#include "CinematicController.h"
#include <graphics/environment/environment3D/IEnvironment3D.h>

using namespace graphics;

CinematicController::CinematicController(std::string& location, IEnvironment3D* environment3D) {
	this->location = location;

	this->environment3D = environment3D;

	dt_worldFocusPoint.set(environment3D->getWorldFocusPoint());
	dt_camera3DZoom = environment3D->getCamera3DZoom();
    dt_cameraPosition.set(environment3D->getCameraPosition());
	dt_cameraDirection.set(environment3D->getCameraDirection());
	dt_sunLightDirection.set(environment3D->getSunLightDirection());
}

void CinematicController::setWorldFocusPoint(float x, float y, float z) {
	// Save locally.
	dt_worldFocusPoint.set(x, y, z);

	if (isActive) {
		sync_worldFocusPoint();
	}
}

void CinematicController::setCamera3DZoom(float cameraZoom) {
	dt_camera3DZoom = cameraZoom;

	if (isActive) {
		sync_camera3DZoom();
	}
}

void CinematicController::setCameraPosition(float dirX, float dirY, float dirZ) {
	// Save locally.
	dt_cameraPosition.set(dirX, dirY, dirZ);

	if (isActive) {
		sync_cameraPosition();
	}
}

void CinematicController::setCameraDirection(float dirX, float dirY, float dirZ) {
	// Save locally.
	dt_cameraDirection.set(dirX, dirY, dirZ);

	if (isActive) {
		sync_cameraDirection();
	}
}

void CinematicController::setSunLightDirection(float dirX, float dirY, float dirZ) {
	// Save locally.
	dt_sunLightDirection.set(dirX, dirY, dirZ);

	if (isActive) {
		sync_sunLightDirection();
	}
}

Camera* CinematicController::peekCamera() {
	return environment3D->peekCamera();
}


int CinematicController::setIsActive(bool isActive) {
	if (this->isActive == isActive) {
		return -1;
	}
	this->isActive = isActive;

	if (this->isActive) {
		sync_worldFocusPoint();
		sync_camera3DZoom();
		sync_cameraDirection();
		sync_sunLightDirection();
	}

	return 0;
}

void CinematicController::sync_worldFocusPoint() {
	environment3D->setWorldFocusPoint(
		dt_worldFocusPoint.x,
		dt_worldFocusPoint.y,
		dt_worldFocusPoint.z
	);
}

void CinematicController::sync_camera3DZoom() {
	environment3D->setCamera3DZoom(dt_camera3DZoom);
}

void CinematicController::sync_cameraPosition() {
	environment3D->setCameraPosition(
		dt_cameraPosition.x,
		dt_cameraPosition.y,
		dt_cameraPosition.z
	);
}

void CinematicController::sync_cameraDirection() {
	environment3D->setCameraDirection(
		dt_cameraDirection.x,
		dt_cameraDirection.y,
		dt_cameraDirection.z
	);
}

void CinematicController::sync_sunLightDirection() {
	environment3D->setSunLightDirection(
		dt_sunLightDirection.x,
		dt_sunLightDirection.y,
		dt_sunLightDirection.z
	);
}

CinematicController::~CinematicController() {
	//void
}
