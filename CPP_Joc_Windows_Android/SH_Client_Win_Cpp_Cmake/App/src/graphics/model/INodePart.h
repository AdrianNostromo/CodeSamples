#pragma once

#include <base/gh.h>
#include <memory>
#include <base/list/ListDL.h>
#include <base/math/Matrix4.h>
#include <base/math/Matrix3.h>

class IListEntry;
namespace graphics {
    class Material;
    class NodeInstance;
    class Node;
    class MeshPart;
    class VertexAttributesList;
    class IRenderablesManager;
};

namespace graphics {
class INodePart {
    pub virtual std::shared_ptr<VertexAttributesList> getVertexAttributesList() = 0;
    pub virtual long getVertexAttributesBitMask() = 0;
    
    pub virtual std::shared_ptr<graphics::Material> getMaterial() = 0;
    pub virtual void setMaterial(std::shared_ptr<Material> material) = 0;

    pub virtual ListDL<INodePart*>::Entry*& getInRenderer_listEntryRef() = 0;

    pub virtual Node* getNode() = 0;
    pub virtual MeshPart* getMeshPartOptional() = 0;

    pub virtual void* getVerticesData() = 0;
    pub virtual int getVerticesCount() = 0;

    pub virtual void* getIndicesData() = 0;
    pub virtual int getIndicesCount() = 0;

    // These are accesses after existence checking. Keep the "must_exist" logic.
    pub virtual Matrix4* getWorldTransformMustExist() = 0;
    pub virtual Matrix4* getNormalWorldTransformMustExist() = 0;

    pub virtual void invalidate(int invalidationBitGroups) = 0;

    pub virtual ~INodePart() = default;
};
};
