#include "MASpotLightsList.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/light/SpotLight.h>
#include <base/MM.h>

using namespace graphics;

std::string MASpotLightsList::Alias = "spotLightsList";
long MASpotLightsList::TypeBitGroup = registerAttribute(Alias);

MASpotLightsList::MASpotLightsList()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

ArrayList<SpotLight*>& MASpotLightsList::peekLightsListManaged() {
	return lightsListManaged;
}

void MASpotLightsList::addLightManaged(SpotLight* light) {
	lightsListManaged.appendDirect(light);
}

void MASpotLightsList::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	ArrayList<SpotLight*>& lightsListManaged = peekLightsListManaged();
	for (int i = 0; i < lightsListManaged.count(); i++) {
		SpotLight* light = lightsListManaged.getDirect(i);

		std::string s = "spotLights[" + std::to_string(i) + "]";

		shaderProgram->setUniformVec3B(s + ".position", light->position, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".direction", light->direction, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".ambient", light->ambient, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".diffuse", light->diffuse, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".specular", light->specular, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".constant", light->constant, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".linear", light->linear, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".quadratic", light->quadratic, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".cutOff", light->cutOffRad, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".outerCutOff", light->outterCutOffRad, false/*mustExist*/);
	}
}

MASpotLightsList* MASpotLightsList::clone(Attribute* _o) {
	// Not implemented currently.
	throw LogicException(LOC);

	/*if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MASpotLightsList* o = newt MASpotLightsList();
	super::clone(o);

	o->texture = texture;

	return o;*/
}

MASpotLightsList::~MASpotLightsList() {
	if (lightsListManaged.count() > 0) {
		for (int i = 0; i < lightsListManaged.count(); i++) {
			SpotLight* light = lightsListManaged.getDirect(i);
		}

		lightsListManaged.clear();
	}
}
