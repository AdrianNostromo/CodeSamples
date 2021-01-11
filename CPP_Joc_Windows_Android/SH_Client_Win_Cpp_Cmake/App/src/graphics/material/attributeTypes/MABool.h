#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/opengl/IGL.h>

namespace graphics {
class MABool : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MABool);
    pub static std::string Alias_LightingStatus;
    pub static long TypeBitGroup_LightingStatus;

    pub GLboolean value;

    pub explicit MABool(long typeBitGroup, GLboolean value);

    pub static MABool* NewLightingStatus(GLboolean value);

    pub MABool* clone(Attribute* o = nullptr) override;

    pub ~MABool() override;
};
};
