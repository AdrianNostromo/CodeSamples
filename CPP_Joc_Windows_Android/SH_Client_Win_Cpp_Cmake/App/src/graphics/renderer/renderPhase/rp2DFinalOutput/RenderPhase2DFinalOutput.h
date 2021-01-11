#pragma once

#include <base/gh.h>
#include "RP2DFO_Base.h"

namespace graphics {
class RenderPhase2DFinalOutput : public RP2DFO_Base {priv typedef RP2DFO_Base super;pub dCtor(RenderPhase2DFinalOutput);
    priv static std::string uniformBlockName_Matrices;
    
    priv UniformBufferObject* uboMatrices = nullptr;
    
    pub explicit RenderPhase2DFinalOutput(
        int renderOrder,
        base::IScreen* screen);
    prot void createMain() override;

    pub void render(graphics::IEnvironment* environment) override;

    prot void prepareShaderUniforms(
        IShader* shader, graphics::IEnvironment* environment,
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms) override;

    prot void dispose() override;
    pub ~RenderPhase2DFinalOutput() override;
};
};
