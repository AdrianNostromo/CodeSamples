#include "EAWorldFocusPosition3D.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/shader/program/ShaderProgram.h>
#include <graphics/camera/Camera.h>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

using namespace graphics;

std::string EAWorldFocusPosition3D::Alias = "worldFocusPosition3D";
long EAWorldFocusPosition3D::TypeBitGroup = registerAttribute(Alias);

EAWorldFocusPosition3D::EAWorldFocusPosition3D()
	: super(TypeBitGroup)
{
	rawSubclassPointer = this;
}

EAWorldFocusPosition3D* EAWorldFocusPosition3D::clone(Attribute* _o) {
	throw LogicException(LOC);// implement when needed.
	/*
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	EAWorldFocusPosition3D* o = newt EAWorldFocusPosition3D();
	super::clone(o);

	return o;*/
}

EAWorldFocusPosition3D::~EAWorldFocusPosition3D() {
	//void
}
