#pragma once

#include <base/gh.h>
#include "E_Base.h"
#include <base/screen/IScreen.h>

namespace graphics {
class E_Camera : public E_Base {priv typedef E_Base super;pub dCtor(E_Camera);
	priv bool isDirty_cameraViewportSize = false;
	priv base::IScreen::CbListener screenListener{"E_Camera"};
	priv int cameraUpdateCounter = 0;

	pub explicit E_Camera();
	prot void createMain() override;

	prot Camera* getCameraMustExist();
	pub Camera* peekCamera() final;
	pub int getCameraUpdateCounter();
	priv void onActiveScreenResolutionChange(int resolutionX, int resolutionY);

	prot void onEnableAtribute(Attribute* attribute) override;
	prot void onDisableAtribute(Attribute* attribute) override;

	prot virtual void onColorCameraViewportSizeChanged();

	pub void syncIfNeeded() override;

	prot void disposeMain() override;

    pub ~E_Camera() override;
};
};
