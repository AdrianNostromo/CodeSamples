#pragma once

#include <base/gh.h>
#include <graphics/camera/Camera.h>

class OrthographicCamera : public Camera {priv typedef Camera super;pub dCtor(OrthographicCamera);
	priv Vector3 tmp{};

    pub explicit OrthographicCamera(bool yDown);

	prot void updateMain() override;

	pub ~OrthographicCamera() override;
};
