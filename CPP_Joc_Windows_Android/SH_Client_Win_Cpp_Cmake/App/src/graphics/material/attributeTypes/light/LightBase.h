#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>

namespace graphics {
class LightBase {pub dCtor(LightBase);
    pub Vector3 ambient{ 1.0f, 1.0f, 1.0f };
    pub Vector3 diffuse{ 1.0f, 1.0f, 1.0f };
    pub Vector3 specular{ 1.0f, 1.0f, 1.0f };

    pub explicit LightBase();

    pub void setAmbient(Vector3& ambient);
    pub void setDiffuse(Vector3& diffuse);
    pub void setSpecular(Vector3& specular);

    pub virtual ~LightBase();
};
};
