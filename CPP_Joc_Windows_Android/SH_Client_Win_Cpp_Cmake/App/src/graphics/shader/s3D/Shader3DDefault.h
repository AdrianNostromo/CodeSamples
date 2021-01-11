#pragma once

#include <base/gh.h>
#include "../Shader.h"

namespace graphics {
class Shader3DDefault : public Shader {priv typedef Shader super;pub dCtor(Shader3DDefault);
    priv static bool isDefaultSourceLoaded;
    priv static std::string defaultFileVSSource;
    priv static std::string defaultFileFSSource;

    priv bool hasNormal3D = false;
    priv bool isDoInShaderDepthMapTextureBorderToMaxDistanceFlag = false;

    priv bool isLightingEnabled = false;

    priv int numDirectionalLights = 0;
    priv int numPointLights = 0;
    priv int numSpotLights = 0;

    pub explicit Shader3DDefault();
    using super::init;
    pub void init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList);

    pub bool canRender(
        IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) override;

    prot std::string generatePrefix() override;

    pub ~Shader3DDefault() override;
};
};
