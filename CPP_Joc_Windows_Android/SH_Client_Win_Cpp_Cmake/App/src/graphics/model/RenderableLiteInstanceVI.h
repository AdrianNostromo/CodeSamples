#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "INodePart.h"
#include <memory>
#include <base/MM.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>

namespace graphics {
    class Material;
};

namespace graphics {
class RenderableLiteInstanceVI : public base::Object, public virtual IRenderable, public virtual IGeometryProvider_Vertices, public virtual IGeometryProvider_Indices {priv typedef base::Object super;pub dCtor(RenderableLiteInstanceVI);
    priv std::shared_ptr<VertexAttributesList> vertexAttributesList;

    //asd_r;// make these 4 priv after testing.
    pub void* verticesUnmanaged;
    pub unsigned int vertices_count = 0;

    pub unsigned short* indicesUnmanaged;
    pub unsigned int indices_count = 0;

    priv std::shared_ptr<Material> material;

    priv Entry_RenderablesManager* entry_renderablesManager = nullptr;

    pub explicit RenderableLiteInstanceVI();
    pub explicit RenderableLiteInstanceVI(
        std::shared_ptr<VertexAttributesList> vertexAttributesList,
        void* vertices, unsigned int vertices_count, 
        unsigned short* indices, unsigned int indices_count,
        std::shared_ptr<Material> material);

    pub void* getVerticesData() final;
    pub VertexBufferObject* getVerticesVBO() final;
    pub int getVerticesCount() final;

    pub void* getIndicesData() final;
    pub IndexBufferObject* getIndicesIBO() final;
    pub int getIndicesOffset() final;
    pub int getIndicesCount() final;

    using IGeometryProvider_Vertices::invalidate;
    using IGeometryProvider_Indices::invalidate;
    // Not sure why the modelTrans and modelNormalTrans are as params here, maybe it is a requirement to have same interface functions for 2d and 3d with or without transforms.
    /// Seems to be removable without a problem but keep for now and remove when Keep it for now as unused code and update if needed in RendererV3 or V4 probably.
    pub void invalidate(
        int invalidationBitMask_Vertices, int invalidationBitMask_Indices, 
        int invalidationBitMask_ModelTransforms, int invalidationBitMask_ModelNormalTransforms);

    pub std::shared_ptr<VertexAttributesList> getVertexAttributesList() final;
    pub void initVertexAttributesList(std::shared_ptr<VertexAttributesList> vertexAttributesList);
    pub long getVertexAttributesBitMask() final;

    pub std::shared_ptr<graphics::Material> getMaterial() final;
    pub void setMaterial(std::shared_ptr<Material> material);

    pub void removeFromRenderer();

    pub Entry_RenderablesManager*& getEntry_renderablesManagerRef() final;

    pub IGeometryProvider_Vertices* getGeometryProvider_Vertices() final;
    pub IGeometryProvider_Indices* getGeometryProvider_Indices() final;
    pub IGeometryProvider_ModelTransforms* getGeometryProvider_ModelTransforms() final;
    
    pub void dispose() override;
    pub ~RenderableLiteInstanceVI() override;
};
};
