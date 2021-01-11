#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
class MAReceiveShadow : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MAReceiveShadow);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub explicit MAReceiveShadow();

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MAReceiveShadow* clone(Attribute* o = nullptr) override;

    pub ~MAReceiveShadow() override;
};
};
