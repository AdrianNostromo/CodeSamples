#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/object/Object.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <base/math/Matrix4.h>
#include <base/math/Matrix3.h>
#include <base/MM.h>

class IListEntry;
namespace graphics {
    class Node;
    class ModelInstance;
};

namespace graphics {
    class NodeInstance : public base::Object/*asd_02;// implement when needed for animated blender models, public virtual IGeometryProvider_ModelTransform*/ {priv typedef base::Object super;pub dCtor(NodeInstance);
    pub class InvalidationFlags {
        pub static const unsigned int anyChildRequest;

        pub static const unsigned int localTransform;
        pub static const unsigned int globalTransform;

        // Any of these flags on the current layer local invalidationBitMap cause the childs to be synced.
        pub static const unsigned int syncAllChildrenMask;
    };
    
    pub Node* node;

    // This can be nullptr for standalone Nodes (eg. 3D fragments rendering).
    pub ModelInstance* modelInstance = nullptr;

    pub NodeInstance* parent = nullptr;
    pub ArrayList<NodeInstance*> childrensList{};

    pub IListEntry* inNode_listEntry = nullptr;

    pub IListEntry* inRenderer_listEntry = nullptr;

    pub unsigned int invalidationBitMap = -1;

    //asd_r_1;// these 2 can be nullptr if not used.
    pub Matrix4 globalTransform{};
    // This is used as a optimisation to not compute it each rendering tick.
    pub Matrix4 tmpMat4{};
    pub Matrix3 globalNormalTransform{};

    pub explicit NodeInstance(
        Node* node, 
        ModelInstance* modelInstance, NodeInstance* parent);

    pub void invalidate(unsigned int invalidationBitMap);
    pub void sync(unsigned int upperInvalidationBitMap);

    pub virtual ~NodeInstance();
};
};
