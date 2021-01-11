#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/opengl/IGL.h>

namespace graphics {
class MADepthTest : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MADepthTest);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub bool isDepthTestEnabled;
    pub GLenum depthFunction;
    pub GLclampd depthRangeNear;
    pub GLclampd depthRangeFar;

    pub explicit MADepthTest(bool isDepthTestEnabled, GLenum depthFunction = IGL::LESS, GLclampd depthRangeNear = 0.0f, GLclampd depthRangeFar = 1.0f);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MADepthTest* clone(Attribute* o = nullptr) override;

    pub ~MADepthTest() override;
};
};
