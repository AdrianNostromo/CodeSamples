#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "INodePart.h"
#include <memory>
#include <base/MM.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>

namespace graphics {
    class MeshPart;
    class Material;
    class Node;
};

// MeshPart is the smallest visible part of a Model.
// Each one is part of a Renderable.
// Each one causes a render call.

namespace graphics {
class NodePart : public base::Object, public virtual INodePart, public virtual IGeometryProvider_Vertices, public virtual IGeometryProvider_Indices {priv typedef base::Object super;pub dCtor(NodePart);
    pub MeshPart* meshPart;
    // This must never be nullptr.
    pub std::shared_ptr<Material> material;

    pub Node* node = nullptr;

    // These are used for standalone NodePart-s (2d with no and textures array) rendering.
    priv ListDL<INodePart*>::Entry* inRenderer_listEntry = nullptr;

    pub explicit NodePart(MeshPart* meshPart, std::shared_ptr<Material> material);

    pub Node* getNode() final;
    pub MeshPart* getMeshPartOptional() final;

    pub void* getVerticesData() final;
    pub VertexBufferObject* getVerticesVBO() final;
    pub int getVerticesCount() final;

    pub void* getIndicesData() final;
    pub IndexBufferObject* getIndicesIBO() final;
    pub int getIndicesOffset() final;
    pub int getIndicesCount() final;

    pub Matrix4* getWorldTransformMustExist() final;
    pub Matrix4* getNormalWorldTransformMustExist() final;

    pub std::shared_ptr<VertexAttributesList> getVertexAttributesList() final;
    pub long getVertexAttributesBitMask() final;

    pub std::shared_ptr<graphics::Material> getMaterial() final;
    pub void setMaterial(std::shared_ptr<Material> material) final;

    pub ListDL<INodePart*>::Entry*& getInRenderer_listEntryRef() final;

    pub void invalidate(int invalidationBitGroups) final;

    pub void dispose() override;
    pub ~NodePart() override;
};
};
