#pragma once

#include <base/gh.h>
#include "RP3DSM_Base.h"

namespace graphics {
    class FrameBufferObject;
};

namespace graphics {
class RP3DShadowMap : public RP3DSM_Base {priv typedef RP3DSM_Base super;pub dCtor(RP3DShadowMap);
    priv static unsigned int SHADOW_WIDTH;
    priv static unsigned int SHADOW_HEIGHT;

    priv static bool IS_DEBUG_SHADOW_MAP;

    priv graphics::FrameBufferObject* fboDepthMap = nullptr;
    priv Texture* depthMapTexture = nullptr;

    pub explicit RP3DShadowMap(
        int renderOrder,
        base::IScreen* screen);

    prot void onIsEnabledChanged() override;
    
    pub void render(graphics::IEnvironment* environment) override;
    
    pub Texture* getShadowDepthMapTexture() final;

    priv void debugRenderShadowMap();

    prot void prepareShaderUniforms(
        IShader* shader, graphics::IEnvironment* environment,
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms) override;

    prot void dispose() override;
    pub ~RP3DShadowMap() override;
};
};
