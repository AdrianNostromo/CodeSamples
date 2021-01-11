#pragma once

#include <base/gh.h>
#include "E3D_Base.h"

namespace graphics {
class E3D_Lighting : public E3D_Base {priv typedef E3D_Base super;pub dCtor(E3D_Lighting);
	// These values are from tests to make the sun area cover the entire visible screen.
	pub static const float BASE_SUNLIGHT_CAMERA_WIDTH;
	pub static const float BASE_SUNLIGHT_CAMERA_HEIGHT;

	pub static const Vector3 defaultSunLightDirection;

	priv Vector3 sunLightDirection{ 0.0f, 0.0f, 0.0f };
	
	pub explicit E3D_Lighting();

	pub virtual Vector3* getSunLightDirection() override { return &sunLightDirection; }
	pub virtual void setSunLightDirection(float dirX, float dirY, float dirZ) override;

	prot void createMain() override;

    pub ~E3D_Lighting() override;
};
};
