#include "Visual3DModelMaterial_Color.h"

Visual3DModelMaterial_Color::Visual3DModelMaterial_Color(
    Color* diffuseColor, bool isBlended)
    :super(),
    diffuseColor(diffuseColor),
    isBlended(isBlended)
{
    //void
}

Visual3DModelMaterial* Visual3DModelMaterial_Color::clone() {
    return new Visual3DModelMaterial_Color(
            diffuseColor
    );
}

Visual3DModelMaterial_Color::~Visual3DModelMaterial_Color() {
    //void
}
