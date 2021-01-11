#include "Attribute.h"
#include <graphics/shader/program/ShaderProgram.h>
#include <base/MM.h>

using namespace graphics;

Attribute::Attribute(long typeBitGroup)
	: typeBitGroup(typeBitGroup)
{
	//void
}

void Attribute::bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) {
	//void
}

Attribute* Attribute::clone(Attribute* o) {
	if (o == nullptr) {
		o = newt Attribute(typeBitGroup);
	}

	return o;
}

Attribute::~Attribute() {
	//void
}
