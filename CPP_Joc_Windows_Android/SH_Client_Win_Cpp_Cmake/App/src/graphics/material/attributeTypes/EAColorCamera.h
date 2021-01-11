#pragma once

#include <base/gh.h>
#include "../MaterialAttribute.h"

class Camera;

namespace graphics {
class EAColorCamera : public MaterialAttribute {priv typedef MaterialAttribute super;pub dCtor(EAColorCamera);
    pub static std::string Alias;
    pub static long TypeBitGroup;

    pub Camera* camera = nullptr;

    priv static std::string u_colorCameraProjectionViewMatrix;

    pub explicit EAColorCamera();

    pub void init(Camera* camera);

    pub void bind(IAttributesList* attributesList, ShaderProgram* shaderProgram) final;

    pub EAColorCamera* clone(Attribute* o = nullptr) override;

    pub ~EAColorCamera() override;
};
};
