#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "INodePart.h"
#include <memory>
#include <base/MM.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>

namespace graphics {
    class Material;
};

namespace graphics {
class RenderableLiteInstanceVIMN : public base::Object, public virtual IRenderable, public virtual IGeometryProvider_Vertices, public virtual IGeometryProvider_Indices, public virtual IGeometryProvider_ModelTransforms {priv typedef base::Object super;pub dCtor(RenderableLiteInstanceVIMN);
    priv std::shared_ptr<VertexAttributesList> vertexAttributesList;

    //asd_r;// make these 4 priv after testing.
    pub void* verticesUnmanaged;
    pub unsigned int vertices_count = 0;

    pub unsigned short* indicesUnmanaged;
    pub unsigned int indices_count = 0;

    priv std::shared_ptr<Material> material;

    // Currently only 1 wTransform may exist per renderable, if needed, an array can be used and a char[vertices_count] that contains the local_wTransform for each vertex.
    priv Matrix4* worldTransform = nullptr;
    // This is suposed to be a Matrix3 but when writing to opengl 3 rows of 4 floats are required because .
    /// Using a Matrix4 and writing 3*4 floats works with a single memcpy call.
    /// The right column and bottom row are not used (the right column is only written as padding).
    priv Matrix4* normalWorldTransform = nullptr;

    priv Entry_RenderablesManager* entry_renderablesManager = nullptr;

    pub explicit RenderableLiteInstanceVIMN();
    pub explicit RenderableLiteInstanceVIMN(
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

    pub UniformBufferObject* getModelTransformsUBO() final;
    pub UniformBufferObject* getRemapingModelTransformsIndicesUBOOptional() final;

    pub UniformBufferObject* getModelNormalTransformsUBO() final;

    pub Matrix4* getWorldTransformOptional() final;
    pub Matrix4* getWorldTransformMustExist() final;
    pub Matrix4* getNormalWorldTransformOptional() final;
    pub Matrix4* getNormalWorldTransformMustExist() final;

    using IGeometryProvider_Vertices::invalidate;
    using IGeometryProvider_Indices::invalidate;
    using IGeometryProvider_ModelTransforms::invalidate;
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
    pub ~RenderableLiteInstanceVIMN() override;
};
};
