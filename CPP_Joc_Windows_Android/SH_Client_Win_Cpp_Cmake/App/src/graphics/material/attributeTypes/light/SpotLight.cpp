#include "SpotLight.h"

using namespace graphics;

SpotLight::SpotLight()
	: super()
{
	//void
}

void SpotLight::setPosition(Vector3& position) {
	this->position.set(position);
}

void SpotLight::setDirection(Vector3& direction) {
	this->direction.set(direction);
}

void SpotLight::setTerms(float constant, float linear, float quadratic) {
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

void SpotLight::setCutOffRad(float cutOffRad, float outterCutOffRad) {
	this->cutOffRad = cutOffRad;
	this->outterCutOffRad = outterCutOffRad;
}

SpotLight::~SpotLight() {
	//void
}
