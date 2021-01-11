#pragma once

#include <base/gh.h>
#include "../Shader.h"

namespace graphics {
class Shader2DDefault : public Shader {priv typedef Shader super;pub dCtor(Shader2DDefault);
    priv static bool isDefaultSourceLoaded;
    priv static std::string defaultFileVSSource;
    priv static std::string defaultFileFSSource;

    pub explicit Shader2DDefault();
    using super::init;
    pub void init(IRenderable* renderable, IEnvironment* environment, IAttributesList* rendererAttributesList);

    pub ~Shader2DDefault() override;
};
};
