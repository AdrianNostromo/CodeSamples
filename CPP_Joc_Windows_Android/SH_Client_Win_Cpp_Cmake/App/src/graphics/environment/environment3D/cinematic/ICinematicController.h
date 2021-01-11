#pragma once

#include <base/math/Vector3.h>
#include <graphics/camera/Camera.h>
#include <memory>

namespace graphics {
	class CinematicController;
};

namespace graphics {
class ICinematicController {
public:
	virtual std::shared_ptr<CinematicController> peekPrev() = 0;

	virtual Vector3* getWorldFocusPoint() = 0;
	virtual void setWorldFocusPoint(float x, float y, float z) = 0;

	virtual float getCamera3DZoom() = 0;
	virtual void setCamera3DZoom(float cameraZoom) = 0;

	virtual Vector3* getCameraPosition() = 0;
	virtual void setCameraPosition(float dirX, float dirY, float dirZ) = 0;

	virtual Vector3* getCameraDirection() = 0;
	virtual void setCameraDirection(float dirX, float dirY, float dirZ) = 0;

	virtual Vector3* getSunLightDirection() = 0;
	virtual void setSunLightDirection(float dirX, float dirY, float dirZ) = 0;

	virtual Camera* peekCamera() = 0;

	virtual ~ICinematicController() = default;
};
};
