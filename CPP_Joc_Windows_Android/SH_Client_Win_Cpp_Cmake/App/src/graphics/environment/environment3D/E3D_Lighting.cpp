#include "E3D_Lighting.h"

using namespace graphics;

const float E3D_Lighting::BASE_SUNLIGHT_CAMERA_WIDTH = 2.8f;
const float E3D_Lighting::BASE_SUNLIGHT_CAMERA_HEIGHT = 2.8f;

const Vector3 E3D_Lighting::defaultSunLightDirection{
	0.0f, 1.0f, -1.0f, true
};


E3D_Lighting::E3D_Lighting()
	: super()
{
	//void
}

void E3D_Lighting::createMain() {
	super::createMain();

	//void
}

void E3D_Lighting::setSunLightDirection(float dirX, float dirY, float dirZ) {
	sunLightDirection.set(
		dirX,
		dirY,
		dirZ
	);
}

E3D_Lighting::~E3D_Lighting() {
	//void
}
