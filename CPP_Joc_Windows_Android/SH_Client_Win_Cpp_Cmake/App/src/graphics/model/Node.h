#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <graphics/model/instance/NodeInstance.h>
#include <base/math/Matrix4.h>
#include <base/MM.h>

namespace graphics {
    class Model;
    class NodePart;
    class ModelInstance;
};

namespace graphics {
class Node : public base::Object {priv typedef base::Object super;pub dCtor(Node);
    pub Model* model = nullptr;

    pub Node* parent = nullptr;
    pub ArrayList<Node*> childrensList{};

    pub ArrayList<NodePart*> partsList{};

    pub ListDL<NodeInstance> instancesList{LOC};

    // These instantly update localTransform.
    priv Vector3 translation{};
    priv Quaternion rotation{};
    priv Vector3 scale{1.0f, 1.0f, 1.0f};

    //asd_r_1;// use ptr and allow for nullptr;
    // This is updated directly when the components change.
    pub Matrix4 localTransform{};
    pub Matrix4 tmpMat4{};
    // This is used for standalon Node-s.
    pub Matrix3* localNormalTransform;

    // These are used for standalone Node-s (3d with transform) rendering.
    pub IListEntry* inRenderer_listEntry = nullptr;

    pub explicit Node(bool computeNormalTransform);

    pub NodePart* appendNodePart(NodePart* nodePart);

    pub NodeInstance* createInstance(ModelInstance* modelInstance, NodeInstance* parentNodeInstance);
    pub void deleteInstance(NodeInstance* nodeInstance);

    pub void setTranslation(float x, float y, float z);
    pub void setRotation(Quaternion& rotation);
    pub void setScale(float scaleX, float scaleY, float scaleZ);

    priv void invalidate(unsigned int invalidationBitMap);

    pub void dispose() override;
    pub ~Node() override;
};
};
