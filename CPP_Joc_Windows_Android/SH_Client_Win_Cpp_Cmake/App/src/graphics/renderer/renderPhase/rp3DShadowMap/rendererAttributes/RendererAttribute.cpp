#include "RendererAttribute.h"

using namespace graphics;

long RendererAttribute::registerAttribute(std::string& alias) {
	static ArrayList<std::string> typesList{};

	int aliasIndex = typesList.indexOf(alias);
	if (aliasIndex >= 0) {
		return 1L << aliasIndex;
	}

	typesList.appendReference(alias);
	long val = 1L << (typesList.count() - 1);

	return val;
}

RendererAttribute::RendererAttribute(long typeBitGroup)
	: super(typeBitGroup)
{
	//void
}

Attribute* RendererAttribute::clone(Attribute* o) {
	if (o == nullptr) {
		// This must be set by a subclass.
		throw LogicException(LOC);
	}
	super::clone(o);

	return o;
}

RendererAttribute::~RendererAttribute() {
	//void
}
