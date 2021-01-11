#pragma once

#include <base/gh.h>
#include <graphics/attribute/IAttributesList.h>
#include <base/object/Object.h>
#include <string>

namespace graphics {
class CombinedAttributesList : public base::Object, public virtual IAttributesList {priv typedef base::Object super;pub dCtor(CombinedAttributesList);
    priv IAttributesList* lists[2];

    priv long bitMask;

    pub explicit CombinedAttributesList(IAttributesList* list1, IAttributesList* list2);

    pub long getBitMask() final;

    pub ArrayList<Attribute*>* peekList() final;

    pub void bind(
        ShaderProgram* shaderProgram,
        long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask);

    pub Attribute* getOptional(long typeBitGroup) final;

    pub ~CombinedAttributesList();
};
};
