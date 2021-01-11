#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
class MACastShadow : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MACastShadow);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub explicit MACastShadow();

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MACastShadow* clone(Attribute* o = nullptr) override;

    pub ~MACastShadow() override;
};
};
