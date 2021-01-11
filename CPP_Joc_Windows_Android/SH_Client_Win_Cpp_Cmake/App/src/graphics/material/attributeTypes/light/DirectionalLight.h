#pragma once

#include <base/gh.h>
#include "LightBase.h"
#include <base/math/Vector3.h>

namespace graphics {
class DirectionalLight : public LightBase {priv typedef LightBase super;pub dCtor(DirectionalLight);
    pub Vector3 direction{ 1.0f, 1.0f, 1.0f };

    pub explicit DirectionalLight();

    pub void setDirection(Vector3& direction);

    pub ~DirectionalLight() override;
};
};
