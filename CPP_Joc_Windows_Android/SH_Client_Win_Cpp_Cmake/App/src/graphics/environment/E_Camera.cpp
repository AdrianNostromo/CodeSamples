#include "E_Camera.h"
#include <base/screen/ScreenUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/camera/Camera.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <cassert>

using namespace graphics;

E_Camera::E_Camera()
	: super()
{
	screenListener.cb_onActiveScreenResolutionChange = std::bind(&E_Camera::onActiveScreenResolutionChange, this, std::placeholders::_1, std::placeholders::_2);
}

void E_Camera::createMain() {
	super::createMain();

	//void
}

Camera* E_Camera::getCameraMustExist() {
	EAColorCamera* eaColorCamera = getMustExistAs<EAColorCamera*>(EAColorCamera::TypeBitGroup);

	if (eaColorCamera == nullptr || eaColorCamera->camera == nullptr) {
		throw LogicException(LOC);
	}

	return eaColorCamera->camera;
}

Camera* E_Camera::peekCamera() {
	return getCameraMustExist();
}

int E_Camera::getCameraUpdateCounter() {
	return cameraUpdateCounter;
}

void E_Camera::onActiveScreenResolutionChange(int resolutionX, int resolutionY) {
	isDirty_cameraViewportSize = true;
}

void E_Camera::onEnableAtribute(Attribute* attribute) {
	super::onEnableAtribute(attribute);

	if (attribute->typeBitGroup == EAColorCamera::TypeBitGroup) {
		if (screenListener.getIsListening()) {
			// Already listening, this should never occur.
			throw LogicException(LOC);
		}

		if (base::ScreenUtil::screen->getIsScreenActive()) {
			isDirty_cameraViewportSize = false;

			Camera* camera = getCameraMustExist();

			camera->setViewportSize(base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY());

			onColorCameraViewportSizeChanged();
		} else {
			isDirty_cameraViewportSize = true;
		}

		base::ScreenUtil::screen->addListener(&screenListener, 2);
	}
}

void E_Camera::onDisableAtribute(Attribute* attribute) {
	super::onDisableAtribute(attribute);

	if (attribute->typeBitGroup == EAColorCamera::TypeBitGroup) {
		if (!screenListener.getIsListening()) {
			// Not listening, this should never occur.
			throw LogicException(LOC);
		}

		base::ScreenUtil::screen->removeListener(&screenListener, 2);
	}
}

void E_Camera::onColorCameraViewportSizeChanged() {
	//void
}

void E_Camera::syncIfNeeded() {
	super::syncIfNeeded();

	Camera* camera = getCameraMustExist();

	if (isDirty_cameraViewportSize) {
		isDirty_cameraViewportSize = false;

		camera->setViewportSize(base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY());

		onColorCameraViewportSizeChanged();
	}
	if (camera->updateIfNeeded()) {
		cameraUpdateCounter++;
	}

	MAShadowMapLight3D* eaShadowMapLight3D = getOptionalAs<MAShadowMapLight3D*>(MAShadowMapLight3D::TypeBitGroup);
	if (eaShadowMapLight3D != nullptr) {
		eaShadowMapLight3D->syncIfNeeded(this);
	}
}

void E_Camera::disposeMain() {
	//void
	
	super::disposeMain();
}

E_Camera::~E_Camera() {
	if (screenListener.getIsListening()) {
		// The listener should have been removed in a base class when the attribute gets disposed_pre;
		assert(false);
	}
}
