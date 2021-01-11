#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

namespace graphics {
class MACastColor : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MACastColor);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub explicit MACastColor();

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MACastColor* clone(Attribute* o = nullptr) override;

    pub ~MACastColor() override;
};
};
