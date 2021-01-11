#include "Material.h"
#include <base/MM.h>

using namespace graphics;

Material::Material()
	: super()
{
	//void
}

Material* Material::clone(AttributesList* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	Material* o = newt Material();
	super::clone(o);

	return o;
}

Material::~Material() {
	reservedDisposeIfNeeded();// Note1001
}
