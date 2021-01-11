#include "RABool.h"
#include <base/MM.h>

using namespace graphics;

std::string RABool::Alias_UseShaderDepthMapBorder = "bool_UseShaderDepthMapBorder";
long RABool::TypeBitGroup_UseShaderDepthMapBorder = registerAttribute(Alias_UseShaderDepthMapBorder);

RABool::RABool(long typeBitGroup, GLboolean value)
	: super(typeBitGroup),
	value(value)
{
	rawSubclassPointer = this;
}

RABool* RABool::NewUseShaderDepthMapBorder(GLboolean value) {
	return new RABool{TypeBitGroup_UseShaderDepthMapBorder, value };
}

RABool* RABool::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	RABool* o = newt RABool(typeBitGroup, value);
	super::clone(o);

	return o;
}

RABool::~RABool() {
	//void
}
