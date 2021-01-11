#include "MAPointLightsList.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/light/PointLight.h>
#include <base/MM.h>

using namespace graphics;

std::string MAPointLightsList::Alias = "pointLightsList";
long MAPointLightsList::TypeBitGroup = registerAttribute(Alias);

MAPointLightsList::MAPointLightsList()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

ArrayList<PointLight*>& MAPointLightsList::peekLightsListManaged() {
	return lightsListManaged;
}

void MAPointLightsList::addLightManaged(PointLight* light) {
	lightsListManaged.appendDirect(light);
}

void MAPointLightsList::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	ArrayList<PointLight*>& lightsListManaged = peekLightsListManaged();
	for (int i = 0; i < lightsListManaged.count(); i++) {
		PointLight* light = lightsListManaged.getDirect(i);

		std::string s = "pointLights[" + std::to_string(i) + "]";

		shaderProgram->setUniformVec3B(s + ".position", light->position, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".ambient", light->ambient, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".diffuse", light->diffuse, false/*mustExist*/);
		shaderProgram->setUniformVec3B(s + ".specular", light->specular, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".constant", light->constant, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".linear", light->linear, false/*mustExist*/);
		shaderProgram->setUniformFloatB(s + ".quadratic", light->quadratic, false/*mustExist*/);
	}
}

MAPointLightsList* MAPointLightsList::clone(Attribute* _o) {
	// Not implemented currently.
	throw LogicException(LOC);

	/*if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MAPointLightsList* o = newt MAPointLightsList();
	super::clone(o);

	o->texture = texture;

	return o;*/
}

MAPointLightsList::~MAPointLightsList() {
	if (lightsListManaged.count() > 0) {
		for (int i = 0; i < lightsListManaged.count(); i++) {
			PointLight* light = lightsListManaged.getDirect(i);
		}

		lightsListManaged.clear();
	}
}
