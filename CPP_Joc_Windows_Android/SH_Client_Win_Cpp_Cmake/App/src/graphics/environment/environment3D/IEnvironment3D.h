#pragma once

#include <base/gh.h>
#include "../IEnvironment.h"
#include <base/math/Vector3.h>
#include <memory>

namespace graphics {
	class ICinematicController;
};

namespace graphics {
class IEnvironment3D : virtual public IEnvironment {
	pub virtual int releaseCinematicController(std::shared_ptr<ICinematicController> target) = 0;

	pub virtual Vector3 const* getWorldFocusPoint() = 0;
	pub virtual void setWorldFocusPoint(float x, float y, float z) = 0;

	pub virtual float getCamera3DZoom() = 0;
	pub virtual void setCamera3DZoom(float camera3DZoom) = 0;

	pub virtual Vector3 const* getCameraPosition() = 0;
	pub virtual void setCameraPosition(float dirX, float dirY, float dirZ) = 0;

	pub virtual Vector3 const* getCameraDirection() = 0;
	pub virtual void setCameraDirection(float dirX, float dirY, float dirZ) = 0;

	pub virtual Vector3 const* getSunLightDirection() = 0;
	pub virtual void setSunLightDirection(float dirX, float dirY, float dirZ) = 0;

    pub virtual ~IEnvironment3D() = default;
};
};
