#include "MAInt.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <base/MM.h>

using namespace graphics;

std::string MAInt::Alias_FrontFace = "int_frontFace";
long MAInt::TypeBitGroup_FrontFace = registerAttribute(Alias_FrontFace);

std::string MAInt::Alias_CullFace = "int_cullFace";
long MAInt::TypeBitGroup_CullFace = registerAttribute(Alias_CullFace);

MAInt::MAInt(long typeBitGroup, GLenum value)
	: super(typeBitGroup),
	value(value)
{
	rawSubclassPointer = this;
}

MAInt* MAInt::NewFrontFace(GLenum value) {
	return new MAInt{TypeBitGroup_FrontFace, value };
}

MAInt* MAInt::NewCullFace(GLenum value) {
	return new MAInt{TypeBitGroup_CullFace, value };
}

void MAInt::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	if (typeBitGroup == TypeBitGroup_CullFace) {
		GLUtil::renderContext->setCullFace(
			true/*isCullFaceEnabled*/,
			value/*cullFaceMode*/
		);
	} else if (typeBitGroup == TypeBitGroup_FrontFace) {
		GLUtil::renderContext->setFrontFace(
			value/*frontFaceMode*/
		);
	}
}

MAInt* MAInt::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MAInt* o = newt MAInt(typeBitGroup, value);
	super::clone(o);

	return o;
}

MAInt::~MAInt() {
	//void
}
