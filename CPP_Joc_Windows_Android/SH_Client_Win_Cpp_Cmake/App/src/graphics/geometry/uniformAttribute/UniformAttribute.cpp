#include "UniformAttribute.h"
#include <base/opengl/IGL.h>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

long UniformAttribute::GetNextTypeBitGroup() {
	static int BIT_INDEX_COUNTER = 0;
	if (BIT_INDEX_COUNTER >= 64) {
		// All bits used.
		throw LogicException(LOC);
	}

	long ret = BoolUtil::setBitAtIndexDirect(0, BIT_INDEX_COUNTER);
	BIT_INDEX_COUNTER++;

	return ret;
}

UniformAttribute::UniformAttribute(
	std::string alias,
	unsigned int dataType, unsigned int count,
	bool mustExistInShader, bool isPadding)
	: alias(alias),
	typeBitGroup(GetNextTypeBitGroup()),
	dataType(dataType), count(count),
	mustExistInShader(mustExistInShader),
	isPadding(isPadding),
	size(count * IGL::sizeOfType(dataType))
{
	//void
}

UniformAttribute::~UniformAttribute() {
	//void
}
