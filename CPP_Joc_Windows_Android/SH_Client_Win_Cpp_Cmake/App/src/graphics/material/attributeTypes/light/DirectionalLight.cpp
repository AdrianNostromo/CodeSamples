#include "DirectionalLight.h"

using namespace graphics;

DirectionalLight::DirectionalLight()
	: super()
{
	//void
}

void DirectionalLight::setDirection(Vector3& direction) {
	this->direction.set(direction);
}

DirectionalLight::~DirectionalLight() {
	//void
}
