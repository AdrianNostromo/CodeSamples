#include "VertexAttribute.h"
#include <base/opengl/IGL.h>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

long VertexAttribute::GetNextTypeBitGroup() {
	static int BIT_INDEX_COUNTER = 0;
	if (BIT_INDEX_COUNTER >= 64) {
		// All bits used.
		throw LogicException(LOC);
	}

	long ret = BoolUtil::setBitAtIndexDirect(0, BIT_INDEX_COUNTER);
	BIT_INDEX_COUNTER++;

	return ret;
}

VertexAttribute::VertexAttribute(
	std::string alias,
	unsigned int dataType, unsigned int count, NormalisationType normalisationType,
	bool mustExistInShader, bool isPadding)
	: alias(alias),
	typeBitGroup(GetNextTypeBitGroup()),
	dataType(dataType), count(count), normalisationType(normalisationType),
	mustExistInShader(mustExistInShader),
	isPadding(isPadding),
	size(count * IGL::sizeOfType(dataType))
{
	//void
}

VertexAttribute::~VertexAttribute() {
	//void
}
