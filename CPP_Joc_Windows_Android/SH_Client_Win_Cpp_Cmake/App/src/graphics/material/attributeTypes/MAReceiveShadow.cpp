#include "MAReceiveShadow.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/attribute/IAttributesList.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/MM.h>

using namespace graphics;

std::string MAReceiveShadow::Alias = "receiveShadow";
long MAReceiveShadow::TypeBitGroup = registerAttribute(Alias);

MAReceiveShadow::MAReceiveShadow()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void MAReceiveShadow::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	MAShadowMapLight3D* maShadowMapLight3D = attributesList->getOptionalAs<MAShadowMapLight3D*>(MAShadowMapLight3D::TypeBitGroup);

	if (maShadowMapLight3D != nullptr) {
		shaderProgram->setUniformVec3(MAShadowMapLight3D::u_shadowLightPos, *maShadowMapLight3D->camera->getPosition(), false/*mustExist*/);
	}
}

MAReceiveShadow* MAReceiveShadow::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MAReceiveShadow* o = newt MAReceiveShadow();
	super::clone(o);

	return o;
}

MAReceiveShadow::~MAReceiveShadow() {
	//void
}
