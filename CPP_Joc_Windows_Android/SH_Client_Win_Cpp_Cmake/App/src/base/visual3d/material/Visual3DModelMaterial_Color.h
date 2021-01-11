#pragma once

#include <base/gh.h>
#include "Visual3DModelMaterial.h"
#include <graphics/visual/Color.h>

class Visual3DModelMaterial_Color : public Visual3DModelMaterial {priv typedef Visual3DModelMaterial super;pub dCtor(Visual3DModelMaterial_Color);
    pub Color* diffuseColor;

    pub bool isBlended;

    pub Visual3DModelMaterial_Color(
        Color* diffuseColor, bool isBlended=false);
    pub Visual3DModelMaterial* clone() override;
    pub ~Visual3DModelMaterial_Color() override;

};
