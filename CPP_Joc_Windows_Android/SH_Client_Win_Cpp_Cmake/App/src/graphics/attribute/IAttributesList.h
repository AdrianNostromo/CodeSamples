#pragma once

#include <base/gh.h>
#include <base/object/IObject.h>
#include <base/list/ArrayList.h>
#include "Attribute.h"

namespace graphics {
    class ShaderProgram;
};

namespace graphics {
class IAttributesList : public base::IObject {
    pub virtual long getBitMask() = 0;

    pub virtual ArrayList<Attribute*>* peekList() = 0;

    pub virtual Attribute* getOptional(long typeBitGroup) = 0;
    
    pub template <typename T>
    T getOptionalAs(long typeBitGroup);
    
    pub template <typename T>
    T getMustExistAs(long typeBitGroup);
    
    pub virtual ~IAttributesList() = default;
};

template <typename T>
T IAttributesList::getOptionalAs(long validationTypeBitGroup) {
    Attribute* attribute = getOptional(validationTypeBitGroup);
    if (attribute == nullptr) {
        return nullptr;
    }

    T cAttribute = attribute->castAs<T>(validationTypeBitGroup);

    return cAttribute;
}

template <typename T>
T IAttributesList::getMustExistAs(long validationTypeBitGroup) {
    Attribute* attribute = getOptional(validationTypeBitGroup);
    if (attribute == nullptr) {
        throw LogicException(LOC);
    }

    T cAttribute = attribute->castAs<T>(validationTypeBitGroup);
    if (cAttribute == nullptr) {
        throw LogicException(LOC);
    }

    return cAttribute;
}

};
