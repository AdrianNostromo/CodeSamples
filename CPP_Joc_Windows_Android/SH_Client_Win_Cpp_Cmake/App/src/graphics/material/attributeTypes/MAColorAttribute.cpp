#include "MAColorAttribute.h"
#include <base/MM.h>

using namespace graphics;

std::string MAColorAttribute::Alias_Diffuse = "colorAttribute_diffuse";
long MAColorAttribute::TypeBitGroup_Diffuse = registerAttribute(Alias_Diffuse);

MAColorAttribute::MAColorAttribute(long typeBitGroup, Color& col)
	: super(typeBitGroup),
	col(col)
{
	rawSubclassPointer = this;
}

MAColorAttribute* MAColorAttribute::NewDiffuse(Color col) {
	return new MAColorAttribute{TypeBitGroup_Diffuse, col};
}

MAColorAttribute* MAColorAttribute::clone(Attribute* _o) {
	if (_o != nullptr) {
		throw LogicException(LOC);
	}

	MAColorAttribute* o = newt MAColorAttribute(typeBitGroup, col);
	super::clone(o);

	return o;
}

MAColorAttribute::~MAColorAttribute() {
	//void
}
