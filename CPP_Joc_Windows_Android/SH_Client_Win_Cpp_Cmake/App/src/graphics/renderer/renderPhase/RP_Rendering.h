#pragma once

#include <base/gh.h>
#include "RP_GeometryBuckets.h"
#include <base/math/Vector2Int.h>

namespace base {
    class IScreen;
};
namespace graphics {
    class UniformBufferObject;
    class IShader;
    class IGeometryProvider_Vertices;
    class IGeometryProvider_Indices;
    class IGeometryProvider_ModelTransforms;
};

namespace graphics {
class RP_Rendering : public RP_GeometryBuckets {priv typedef RP_GeometryBuckets super;pub dCtor(RP_Rendering);
    prot class Capabilities {
        pub bool isInit = false;

        // Only some capabilities that are optional are checked.
        // graphics::MABlending::TypeBitGroup
        pub int materialAttributes = 0;

        //asd_01;// add a capability for remapingModelTransformsIndices
        
        pub bool normalMatrix = false;

        pub void init(int materialAttributes, bool normalMatrix) {
            this->materialAttributes = materialAttributes;
            this->normalMatrix = normalMatrix;

            isInit = true;
        }
    };

    priv static std::string u_modelMatrix;
    priv static std::string uniformBlockName_ModelTransforms;
    priv static int uniformBlockIndex_ModelTransforms;

    priv static std::string u_modelNormalMatrix;
    priv static std::string uniformBlockName_ModelNormalTransforms;
    priv static int uniformBlockIndex_ModelNormalTransforms;

    priv static std::string uniformBlockName_RemapingModelTransformsIndices;
    priv static int uniformBlockIndex_RemapingModelTransformsIndices;

    // This is needed for the screen viewport size.
    prot base::IScreen* screen;

    // These are used to automatically set some attributes.
    priv long supportedAutomaticAttributesBitMask;
    // This is for bug detection.
    // This is used to catch attributes that maybe should be supported but were not added to the above mask.
    priv long ignorredAutomaticAttributesBitMask;

    prot Capabilities capabilities{};

    // Used for shadow map texture size change
    prot Vector2Int viewportSizeOverride{-1, -1};

    pub explicit RP_Rendering(
        int renderOrder, bool useDepthTest,
        base::IScreen* screen,
        long supportedAutomaticAttributesBitMask, long ignorredAutomaticAttributesBitMask);
    prot void createMain() override;

    pub void render(graphics::IEnvironment* environment) override;
    pub void renderRenderable(IRenderable* renderable, IShader*& activeShader, graphics::IEnvironment* environment);
    prot virtual void prepareShaderUniforms(
        IShader* shader, graphics::IEnvironment* environment, 
        IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
        IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms
    );

    prot void dispose() override;
    pub ~RP_Rendering() override;
};
};
