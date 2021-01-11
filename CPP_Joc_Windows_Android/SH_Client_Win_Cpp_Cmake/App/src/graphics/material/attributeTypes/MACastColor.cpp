#include "MACastColor.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/attribute/IAttributesList.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/MM.h>

using namespace graphics;

std::string MACastColor::Alias = "castColor";
long MACastColor::TypeBitGroup = registerAttribute(Alias);

MACastColor::MACastColor()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void MACastColor::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	//void
}

MACastColor* MACastColor::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MACastColor* o = newt MACastColor();
	super::clone(o);

	return o;
}

MACastColor::~MACastColor() {
	//void
}
