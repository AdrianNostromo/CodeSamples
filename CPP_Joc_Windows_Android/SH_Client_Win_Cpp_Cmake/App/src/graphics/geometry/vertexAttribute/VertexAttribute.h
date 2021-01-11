#pragma once

#include <base/gh.h>
#include <string>

namespace graphics {
class VertexAttribute {pub dCtor(VertexAttribute);
	// These enume types control which function (vertexAttribPointer ir vertexAttribIPointer) is used to prepare the attributes.
	// NO_floatConversion - No normalisation is used and the type provided will be converted to a <<<<<FLOAT>>>>> in the shader.
	// NO_integerType - No normalisation is used and the type provided will be converted to a <<<<<INTEGER_TYPE>>>>> type in the shader.
	// YES - Normalisation is used and the type provided will be converted to a <<<<<FLOAT>>>>> in the shader. Any type provided will be converted to [-1, 1] for signed types and [0, 1] for unsigned types.
	pub enum NormalisationType { NO_floatConversion, NO_integerType, YES_floatConversion};

	priv static long GetNextTypeBitGroup();

	pub std::string alias;

	pub long typeBitGroup;

	pub unsigned int dataType;
	pub unsigned int count;
	pub NormalisationType normalisationType;

	pub bool mustExistInShader;
	pub bool isPadding;

	// size = count * size_of_dataType.
	pub unsigned int size;

	pub explicit VertexAttribute(
		std::string alias,
		unsigned int dataType, unsigned int count, NormalisationType normalisationType,
		bool mustExistInShader = false, bool isPadding = false);

    pub virtual ~VertexAttribute();
};
};
