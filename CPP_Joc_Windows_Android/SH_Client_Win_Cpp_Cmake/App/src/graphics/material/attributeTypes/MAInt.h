#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/opengl/IGL.h>

namespace graphics {
class MAInt : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MAInt);
    pub static std::string Alias_FrontFace;
    pub static long TypeBitGroup_FrontFace;

    pub static std::string Alias_CullFace;
    pub static long TypeBitGroup_CullFace;

    pub GLenum value;

    pub explicit MAInt(long typeBitGroup, GLenum value);

    pub static MAInt* NewFrontFace(GLenum value);
    pub static MAInt* NewCullFace(GLenum value);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MAInt* clone(Attribute* o = nullptr) override;

    pub ~MAInt() override;
};
};
