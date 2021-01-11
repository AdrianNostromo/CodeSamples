#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"
#include <base/opengl/IGL.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2.h>

class OrthographicCamera;

namespace graphics {
class MAShadowMapLight3D : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(MAShadowMapLight3D);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    priv static std::string u_shadowLightProjectionViewMatrix;
    pub static std::string u_shadowLightPos;

    priv float distanceRange_min;
    priv float distanceRange_max;

    priv Vector2 lightViewPortSize;

    pub OrthographicCamera* camera;

    // This is the offset from the world focus position (player pos).
    pub float distToFocusPos;
    pub Vector3 direction{};

    pub explicit MAShadowMapLight3D(
        float distanceRange_min, float distanceRange_max,
        Vector2 lightViewPortSize);

    pub void syncIfNeeded(IAttributesList* attributesList);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub MAShadowMapLight3D* clone(Attribute* o = nullptr) override;

    pub ~MAShadowMapLight3D() override;
};
};
