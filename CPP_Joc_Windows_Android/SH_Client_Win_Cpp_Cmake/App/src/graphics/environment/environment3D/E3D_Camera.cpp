#include "E3D_Camera.h"
#include <graphics/camera/PerspectiveCamera.h>
#include <graphics/material/attributeTypes/EAWorldFocusPosition3D.h>

using namespace graphics;

E3D_Camera::E3D_Camera()
	: super()
{
	//void
}

Vector3 const* E3D_Camera::getWorldFocusPoint() {
	EAWorldFocusPosition3D* eaWorldFocusPosition3D = getMustExistAs<EAWorldFocusPosition3D*>(EAWorldFocusPosition3D::TypeBitGroup);

	return &eaWorldFocusPosition3D->focusPos;
}

void E3D_Camera::setWorldFocusPoint(float x, float y, float z) {
	EAWorldFocusPosition3D* eaWorldFocusPosition3D = getMustExistAs<EAWorldFocusPosition3D*>(EAWorldFocusPosition3D::TypeBitGroup);

	if (eaWorldFocusPosition3D->focusPos.z == x && eaWorldFocusPosition3D->focusPos.y == y && eaWorldFocusPosition3D->focusPos.z == z) {
		return;
	}

	eaWorldFocusPosition3D->focusPos.set(x, y, z);
}

Vector3 const* E3D_Camera::getCameraPosition() {
	return getCameraMustExist()->getPosition();
}

void E3D_Camera::setCameraPosition(float posX, float posY, float posZ) {
	getCameraMustExist()->setPosition(posX, posY, posZ);
}

Vector3 const* E3D_Camera::getCameraDirection() {
	return getCameraMustExist()->getDirection();
}

void E3D_Camera::setCameraDirection(float dirX, float dirY, float dirZ) {
	getCameraMustExist()->setDirection(dirX, dirY, dirZ);
}

float E3D_Camera::getCamera3DZoom() {
	return getCameraMustExist()->getViewportScaleZoom();
}

void E3D_Camera::setCamera3DZoom(float camera3DZoom) {
	getCameraMustExist()->setViewportScaleZoom(camera3DZoom);
}

E3D_Camera::~E3D_Camera() {
	//void
}
