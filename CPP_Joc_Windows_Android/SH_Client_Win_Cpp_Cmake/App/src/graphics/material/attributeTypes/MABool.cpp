#include "MABool.h"
#include <base/MM.h>

using namespace graphics;

std::string MABool::Alias_LightingStatus = "bool_lightingStatus";
long MABool::TypeBitGroup_LightingStatus = registerAttribute(Alias_LightingStatus);

MABool::MABool(long typeBitGroup, GLboolean value)
	: super(typeBitGroup),
	value(value)
{
	rawSubclassPointer = this;
}

MABool* MABool::NewLightingStatus(GLboolean value) {
	return new MABool{TypeBitGroup_LightingStatus, value };
}

MABool* MABool::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MABool* o = newt MABool(typeBitGroup, value);
	super::clone(o);

	return o;
}

MABool::~MABool() {
	//void
}
