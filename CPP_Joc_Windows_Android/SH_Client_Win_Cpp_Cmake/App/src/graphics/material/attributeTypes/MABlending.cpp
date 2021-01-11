#include "MABlending.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <base/MM.h>

using namespace graphics;

std::string MABlending::Alias = "blending";
long MABlending::TypeBitGroup = registerAttribute(Alias);

MABlending::MABlending(bool isBlendingEnabled, GLenum blendingSFactor, GLenum blendingDFactor)
	: super(TypeBitGroup),
	isBlendingEnabled(isBlendingEnabled), blendingSFactor(blendingSFactor), blendingDFactor(blendingDFactor)
{
	rawSubclassPointer = this;
}

void MABlending::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	if (isBlendingEnabled) {
		GLUtil::renderContext->setBlending(true, blendingSFactor, blendingDFactor);
	}
}

MABlending* MABlending::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MABlending* o = newt MABlending(isBlendingEnabled, blendingSFactor, blendingDFactor);
	super::clone(o);

	return o;
}

MABlending::~MABlending() {
	//void
}
