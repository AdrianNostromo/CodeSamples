#include "MADirectionalLightsList.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/light/DirectionalLight.h>
#include <base/MM.h>

using namespace graphics;

std::string MADirectionalLightsList::Alias = "directionalLightsList";
long MADirectionalLightsList::TypeBitGroup = registerAttribute(Alias);

MADirectionalLightsList::MADirectionalLightsList()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

ArrayList<DirectionalLight*>& MADirectionalLightsList::peekLightsListManaged() {
	return lightsListManaged;
}

void MADirectionalLightsList::addLightManaged(DirectionalLight* light) {
	lightsListManaged.appendDirect(light);
}

void MADirectionalLightsList::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	for (int i = 0; i < lightsListManaged.count(); i++) {
		DirectionalLight* light = lightsListManaged.getDirect(i);

		std::string s = "dirLights[" + std::to_string(i) + "]";

		shaderProgram->setUniformVec3B(s + ".direction", light->direction, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".ambient", light->ambient, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".diffuse", light->diffuse, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".specular", light->specular, false/*mustExist*/);
	}
}

MADirectionalLightsList* MADirectionalLightsList::clone(Attribute* _o) {
	// Not implemented currently.
	throw LogicException(LOC);

	/*if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MADirectionalLightsList* o = newt MADirectionalLightsList();
	super::clone(o);

	o->texture = texture;

	return o;*/
}

MADirectionalLightsList::~MADirectionalLightsList() {
	if (lightsListManaged.count() > 0) {
		for (int i = 0; i < lightsListManaged.count(); i++) {
			DirectionalLight* light = lightsListManaged.getDirect(i);
		}

		lightsListManaged.clear();
	}
}
