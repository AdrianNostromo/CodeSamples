#pragma once

#include <base/gh.h>
#include <string>

namespace graphics {
class UniformAttribute {pub dCtor(UniformAttribute);
	priv static long GetNextTypeBitGroup();

	pub std::string alias;

	pub long typeBitGroup;

	pub unsigned int dataType;
	pub unsigned int count;

	pub bool mustExistInShader;
	pub bool isPadding;

	// size = count * size_of_dataType.
	pub unsigned int size;

	pub explicit UniformAttribute(
		std::string alias,
		unsigned int dataType, unsigned int count,
		bool mustExistInShader = false, bool isPadding = false);

    pub virtual ~UniformAttribute();
};
};
