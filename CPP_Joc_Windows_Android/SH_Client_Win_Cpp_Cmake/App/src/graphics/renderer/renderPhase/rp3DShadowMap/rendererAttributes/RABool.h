#pragma once

#include <base/gh.h>
#include "RendererAttribute.h"
#include <base/opengl/IGL.h>

namespace graphics {
class RABool : public RendererAttribute {priv typedef RendererAttribute super;pub dCtor(RABool);
    pub static std::string Alias_UseShaderDepthMapBorder;
    pub static long TypeBitGroup_UseShaderDepthMapBorder;

    pub GLboolean value;

    pub explicit RABool(long typeBitGroup, GLboolean value);

    pub static RABool* NewUseShaderDepthMapBorder(GLboolean value);

    pub RABool* clone(Attribute* o = nullptr) override;

    pub ~RABool() override;
};
};
