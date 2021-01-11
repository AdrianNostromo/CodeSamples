#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

class Texture;

namespace graphics {
class MADiffuseTexture : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MADiffuseTexture);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv static std::string u_texture;

    Texture* texture = nullptr;

    pub explicit MADiffuseTexture();

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MADiffuseTexture* clone(Attribute* o = nullptr) override;

    pub ~MADiffuseTexture() override;
};
};
