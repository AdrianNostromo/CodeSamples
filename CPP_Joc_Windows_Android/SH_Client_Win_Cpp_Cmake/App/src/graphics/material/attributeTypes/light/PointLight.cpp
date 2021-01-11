#include "PointLight.h"

using namespace graphics;

PointLight::PointLight()
	: super()
{
	//void
}

void PointLight::setPosition(Vector3& position) {
	this->position.set(position);
}

void PointLight::setTerms(float constant, float linear, float quadratic) {
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

PointLight::~PointLight() {
	//void
}
