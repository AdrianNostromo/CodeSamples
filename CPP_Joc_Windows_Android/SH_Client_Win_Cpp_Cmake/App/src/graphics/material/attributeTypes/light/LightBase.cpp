#include "LightBase.h"

using namespace graphics;

LightBase::LightBase()
{
	//void
}

void LightBase::setAmbient(Vector3& ambient) {
	this->ambient.set(ambient);
}

void LightBase::setDiffuse(Vector3& diffuse) {
	this->diffuse.set(diffuse);
}

void LightBase::setSpecular(Vector3& specular) {
	this->specular.set(specular);
}

LightBase::~LightBase() {
	//void
}
