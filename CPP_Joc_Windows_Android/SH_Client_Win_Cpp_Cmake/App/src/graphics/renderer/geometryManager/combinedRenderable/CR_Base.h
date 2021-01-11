#pragma once

#include <base/gh.h>
#include "ICombinedRenderable.h"
#include <base/object/ManagedObject.h>
#include <base/list/ListDL.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <base/list/Array1D.h>
#include <memory>
#include <base/MM.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>

namespace graphics {
    class VertexAttributesList;
    class GeometryProviderInstance;
    class GeometryBufferManager;
}

namespace graphics {
class CR_Base : public base::ManagedObject, public virtual ICombinedRenderable, public virtual IRenderable, public virtual IGeometryProvider_Vertices, public virtual IGeometryProvider_Indices, public virtual IGeometryProvider_ModelTransforms {priv typedef base::ManagedObject super;pub dCtor(CR_Base);
    priv std::shared_ptr<VertexAttributesList> vertexAttributesList;
    prot std::shared_ptr<graphics::Material> material;
    priv int maxVerticesCount;
    priv int maxIndicesCount;
    priv int maxWorldTransformsCount;

    pub GeometryBufferManager* mm_vertices = nullptr;
    pub GeometryBufferManager* mm_indices = nullptr;
    pub GeometryBufferManager* mm_modelTransforms = nullptr;
    // Uses a 16bit compression logic, this will give double the entries count.
    pub GeometryBufferManager* mm_remapingModelTransformsIndices = nullptr;

    // This is the max of the mem manager values.
    // This is kept updated when the mem man values are changed.
    priv float fillRatePercent = 0.0f;

    pub explicit CR_Base(
        std::shared_ptr<VertexAttributesList> vertexAttributesList, 
        std::shared_ptr<graphics::Material> material,
        int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount);
    pub void create() override;

    pub float getFillRatePercent();

    pub std::shared_ptr<graphics::Material> getMaterial() final;

    pub std::shared_ptr<VertexAttributesList> getVertexAttributesList() final;
    pub long getVertexAttributesBitMask() final;

    pub void* getVerticesData() final;
    pub VertexBufferObject* getVerticesVBO() final;
    pub int getVerticesCount() final;

    pub void* getIndicesData() final;
    pub IndexBufferObject* getIndicesIBO() final;
    pub int getIndicesOffset() final;

    pub UniformBufferObject* getModelTransformsUBO() final;
    pub UniformBufferObject* getRemapingModelTransformsIndicesUBOOptional() final;

    pub UniformBufferObject* getModelNormalTransformsUBO() final;

    pub Matrix4* getWorldTransformOptional() final;
    pub Matrix4* getWorldTransformMustExist() final;
    pub Matrix4* getNormalWorldTransformOptional() final;
    pub Matrix4* getNormalWorldTransformMustExist() final;

    pub Entry_RenderablesManager*& getEntry_renderablesManagerRef() final;

    pub IGeometryProvider_Vertices* getGeometryProvider_Vertices() final;
    pub IGeometryProvider_Indices* getGeometryProvider_Indices() final;
    
    pub IGeometryProvider_ModelTransforms* getGeometryProvider_ModelTransforms() final;

    pub void onMemManFillRateCHanged() final;

    pub void tickDefrag();

    pub void dispose() override;
    pub ~CR_Base() override;
};
};
