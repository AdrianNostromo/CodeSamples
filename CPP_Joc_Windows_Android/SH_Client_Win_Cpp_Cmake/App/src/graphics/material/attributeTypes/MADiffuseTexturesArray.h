#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/list/ArrayList.h>

namespace graphics {
    class TextureArray;
};

namespace graphics {
class MADiffuseTexturesArray : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MADiffuseTexturesArray);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv static std::string u_textureArrayArray;

    pub ArrayList<graphics::TextureArray*>* globalTextures;

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub explicit MADiffuseTexturesArray();

    pub MADiffuseTexturesArray* clone(Attribute* o = nullptr) override;

    pub ~MADiffuseTexturesArray() override;
};
};
