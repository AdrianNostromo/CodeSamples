#pragma once

#include <base/gh.h>
#include "LightBase.h"
#include <base/math/Vector3.h>

namespace graphics {
class SpotLight : public LightBase {priv typedef LightBase super;pub dCtor(SpotLight);
    pub Vector3 position{ 1.0f, 1.0f, 1.0f };
    pub Vector3 direction{ 1.0f, 1.0f, 1.0f };

    pub float constant = 1.0f;
    pub float linear = 1.0f;
    pub float quadratic = 1.0f;

    pub float cutOffRad = 0.0f;
    pub float outterCutOffRad = 0.0f;

    pub explicit SpotLight();

    pub void setPosition(Vector3& position);
    pub void setDirection(Vector3& direction);

    pub void setTerms(float constant, float linear, float quadratic);

    pub void setCutOffRad(float cutOffRad, float outterCutOffRad);

    pub ~SpotLight() override;
};
};
