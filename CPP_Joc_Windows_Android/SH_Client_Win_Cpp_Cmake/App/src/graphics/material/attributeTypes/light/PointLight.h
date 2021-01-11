#pragma once

#include <base/gh.h>
#include "LightBase.h"
#include <base/math/Vector3.h>

namespace graphics {
class PointLight : public LightBase {priv typedef LightBase super;pub dCtor(PointLight);
    pub Vector3 position{ 1.0f, 1.0f, 1.0f };

    pub float constant = 1.0f;
    pub float linear = 1.0f;
    pub float quadratic = 1.0f;

    pub explicit PointLight();

    pub void setPosition(Vector3& position);

    pub void setTerms(float constant, float linear, float quadratic);

    pub ~PointLight() override;
};
};
