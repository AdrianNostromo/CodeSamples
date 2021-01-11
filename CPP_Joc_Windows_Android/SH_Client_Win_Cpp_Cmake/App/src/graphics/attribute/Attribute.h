#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>
#include <string>
#include <base/list/ArrayList.h>

namespace graphics {
	class ShaderProgram;
	class IAttributesList;
};

namespace graphics {
class Attribute {pub dCtor(Attribute);
    pub long typeBitGroup;

	// This is used to avoid dynamic cast.
	// It contains the subclass void*;
	prot void* rawSubclassPointer = nullptr;

    pub explicit Attribute(long typeBitGroup);

	// Extend if needed.
	pub virtual void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram);

    template <typename T>
    T castAs(long validationTypeBitGroup);

	pub virtual Attribute* clone(Attribute* o=nullptr);

    pub virtual ~Attribute();
};

template <typename T>
T Attribute::castAs(long validationTypeBitGroup) {
	if (this->typeBitGroup != validationTypeBitGroup || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawSubclassPointer);

	return cVal;
}

};
