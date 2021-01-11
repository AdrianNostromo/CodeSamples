#include "MACastShadow.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/attribute/IAttributesList.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/MM.h>

using namespace graphics;

std::string MACastShadow::Alias = "castShadow";
long MACastShadow::TypeBitGroup = registerAttribute(Alias);

MACastShadow::MACastShadow()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void MACastShadow::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	//void
}

MACastShadow* MACastShadow::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MACastShadow* o = newt MACastShadow();
	super::clone(o);

	return o;
}

MACastShadow::~MACastShadow() {
	//void
}
