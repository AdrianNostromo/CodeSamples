#include "EAColorCamera.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/camera/Camera.h>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

using namespace graphics;

std::string EAColorCamera::Alias = "colorCamera";
long EAColorCamera::TypeBitGroup = registerAttribute(Alias);

std::string EAColorCamera::u_colorCameraProjectionViewMatrix = "u_colorCameraProjectionViewMatrix";

EAColorCamera::EAColorCamera()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

void EAColorCamera::init(Camera* camera) {
	if (this->camera != nullptr || camera == nullptr) {
		throw LogicException(LOC);
	}

	this->camera = camera;
}

void EAColorCamera::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	super::bind(attributesList, shaderProgram);

	shaderProgram->setUniformMatrix4Optional(u_colorCameraProjectionViewMatrix, camera->getCombined());
}

EAColorCamera* EAColorCamera::clone(Attribute* _o) {
	throw LogicException(LOC);// implement when needed.
	/*
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	EAColorCamera* o = newt EAColorCamera();
	super::clone(o);

	return o;*/
}

EAColorCamera::~EAColorCamera() {
	if (camera) {
		delete camera;

		camera = nullptr;
	}
}
