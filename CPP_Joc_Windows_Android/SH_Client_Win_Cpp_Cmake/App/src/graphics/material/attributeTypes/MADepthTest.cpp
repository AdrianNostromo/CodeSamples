#include "MADepthTest.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <base/MM.h>

using namespace graphics;

std::string MADepthTest::Alias = "depthTest";
long MADepthTest::TypeBitGroup = registerAttribute(Alias);

MADepthTest::MADepthTest(bool isDepthTestEnabled, GLenum depthFunction, GLclampd depthRangeNear, GLclampd depthRangeFar)
	: super(TypeBitGroup),
	isDepthTestEnabled(isDepthTestEnabled), depthFunction(depthFunction), depthRangeNear(depthRangeNear), depthRangeFar(depthRangeFar)
{
	rawSubclassPointer = this;
}

void MADepthTest::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	if (isDepthTestEnabled) {
		GLUtil::renderContext->setDepthTest(true, depthFunction, depthRangeNear, depthRangeFar);
	}
}

MADepthTest* MADepthTest::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MADepthTest* o = newt MADepthTest(isDepthTestEnabled, depthFunction, depthRangeNear, depthRangeFar);
	super::clone(o);

	return o;
}

MADepthTest::~MADepthTest() {
	//void
}
