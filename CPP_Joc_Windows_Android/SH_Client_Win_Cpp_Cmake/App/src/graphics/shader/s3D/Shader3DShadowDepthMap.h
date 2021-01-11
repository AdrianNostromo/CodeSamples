#pragma once

#include <base/gh.h>
#include "../Shader.h"

namespace graphics {
class Shader3DShadowDepthMap : public Shader {priv typedef Shader super;pub dCtor(Shader3DShadowDepthMap);
    priv static bool isDefaultSourceLoaded;
    priv static std::string defaultFileVSSource;
    priv static std::string defaultFileFSSource;

    pub explicit Shader3DShadowDepthMap();
    using super::init;
    pub void init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList);

    pub bool canRender(
        IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList) override;

    prot std::string generatePrefix() override;

    pub ~Shader3DShadowDepthMap() override;
};
};
