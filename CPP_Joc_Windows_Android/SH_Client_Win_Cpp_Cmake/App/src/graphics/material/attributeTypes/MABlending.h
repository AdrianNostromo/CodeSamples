#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/opengl/IGL.h>

namespace graphics {
class MABlending : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MABlending);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub bool isBlendingEnabled;
    pub GLenum blendingSFactor;
    pub GLenum blendingDFactor;

    pub explicit MABlending(bool isBlendingEnabled, GLenum blendingSFactor, GLenum blendingDFactor);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MABlending* clone(Attribute* o = nullptr) override;

    pub ~MABlending() override;
};
};
