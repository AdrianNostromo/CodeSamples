#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/math/Vector3.h>

class Camera;

namespace graphics {
class EAWorldFocusPosition3D : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(EAWorldFocusPosition3D);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    // This is usually the player pos or player unit center of gravity.
    pub Vector3 focusPos{0.0f, 0.0f, 0.0f};

    pub explicit EAWorldFocusPosition3D();

    pub EAWorldFocusPosition3D* clone(Attribute* o = nullptr) override;

    pub ~EAWorldFocusPosition3D() override;
};
};
