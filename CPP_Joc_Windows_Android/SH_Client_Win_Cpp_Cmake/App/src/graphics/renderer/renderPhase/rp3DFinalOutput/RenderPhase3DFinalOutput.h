#pragma once

#include <base/gh.h>
#include "RP3DFO_Base.h"

namespace graphics {
    class IRP3DShadowMap;
};

namespace graphics {
class RenderPhase3DFinalOutput : public RP3DFO_Base {priv typedef RP3DFO_Base super;pub dCtor(RenderPhase3DFinalOutput);
    priv static std::string uniformBlockName_Matrices;
    priv static int uniformBlockIndex_Matrices;

    priv static std::string u_shadowDepthMapTexture;
    
    priv UniformBufferObject* uboMatrices = nullptr;

    priv IRP3DShadowMap* rp3DShadowMap = nullptr;
    
    pub explicit RenderPhase3DFinalOutput(
        int renderOrder,
        base::IScreen* screen);
    prot void createMain() override;

    prot void onIsEnabledChanged() override;

    pub void render(graphics::IEnvironment* environment) override;

    prot void prepareShaderUniforms(
        IShader* shader, graphics::IEnvironment* environment,
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms) override;

    prot void dispose() override;
    pub ~RenderPhase3DFinalOutput() override;
};
};
