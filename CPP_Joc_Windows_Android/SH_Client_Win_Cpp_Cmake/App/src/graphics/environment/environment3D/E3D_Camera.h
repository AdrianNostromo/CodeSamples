#pragma once

#include <base/gh.h>
#include "E3D_Lighting.h"

namespace graphics {
class E3D_Camera : public E3D_Lighting {priv typedef E3D_Lighting super;pub dCtor(E3D_Camera);
	// The position where to center the drop_shadow, distance_fog, and other stuff;
	pub explicit E3D_Camera();

	pub Vector3 const* getWorldFocusPoint() override;
	pub void setWorldFocusPoint(float x, float y, float z) override;

	pub Vector3 const* getCameraPosition() override;
	pub void setCameraPosition(float posX, float posY, float posZ) override;

	pub Vector3 const* getCameraDirection() override;
	pub void setCameraDirection(float dirX, float dirY, float dirZ) override;

	pub float getCamera3DZoom() override;
	pub void setCamera3DZoom(float camera3DZoom) override;

    pub ~E3D_Camera() override;
};
};
