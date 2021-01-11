#include "MAShadowMapLight3D.h"
#include <graphics/camera/OrthographicCamera.h>
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/material/attributeTypes/EAWorldFocusPosition3D.h>
#include <graphics/attribute/IAttributesList.h>
#include <base/MM.h>

using namespace graphics;

std::string MAShadowMapLight3D::Alias = "shadowMapLight3D";
long MAShadowMapLight3D::TypeBitGroup = registerAttribute(Alias);

std::string MAShadowMapLight3D::u_shadowLightProjectionViewMatrix = "u_shadowLightProjectionViewMatrix";
std::string MAShadowMapLight3D::u_shadowLightPos = "u_shadowLightPos";

MAShadowMapLight3D::MAShadowMapLight3D(
	float distanceRange_min, float distanceRange_max,
	Vector2 lightViewPortSize)
	: super(TypeBitGroup),
	distanceRange_min(distanceRange_min), distanceRange_max(distanceRange_max),
	lightViewPortSize(std::move(lightViewPortSize))
{
	rawSubclassPointer = this;

	camera = new OrthographicCamera(false/*yDown*/);
}

void MAShadowMapLight3D::syncIfNeeded(IAttributesList* attributesList) {
	// Get the camera attribute and the focus pos attribute.
	EAColorCamera* eaColorCamera = attributesList->getMustExistAs<EAColorCamera*>(EAColorCamera::TypeBitGroup);
	EAWorldFocusPosition3D* eaWorldFocusPosition3D = attributesList->getMustExistAs<EAWorldFocusPosition3D*>(EAWorldFocusPosition3D::TypeBitGroup);

	Vector3 pos{
		eaWorldFocusPosition3D->focusPos.x - direction.x * distToFocusPos,
		eaWorldFocusPosition3D->focusPos.y - direction.y * distToFocusPos,
		eaWorldFocusPosition3D->focusPos.z - direction.z * distToFocusPos
	};
	camera->setPosition(pos.x, pos.y, pos.z);

	camera->setDirection(direction.x, direction.y, direction.z);

	camera->setDistanceRange(distanceRange_min, distanceRange_max);
	camera->setViewportSize(lightViewPortSize.x, lightViewPortSize.y);

	camera->updateIfNeeded();
}

void MAShadowMapLight3D::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	shaderProgram->setUniformMatrix4Optional(u_shadowLightProjectionViewMatrix, camera->getCombined());
}

MAShadowMapLight3D* MAShadowMapLight3D::clone(Attribute* _o) {
	// Not implemented currently.
	throw LogicException(LOC);

	/*if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MAShadowMapLight3D* o = newt MAShadowMapLight3D();
	super::clone(o);

	return o;*/
}

MAShadowMapLight3D::~MAShadowMapLight3D() {
	if (camera != nullptr) {
		delete camera;
		camera = nullptr;
	}
}
