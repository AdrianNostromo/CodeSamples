#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <graphics/model/instance/ModelInstance.h>
#include <base/MM.h>

namespace graphics {
    class Node;
    class Mesh;
    class NodePart;
};

namespace graphics {
class Model : public base::Object {priv typedef base::Object super;pub dCtor(Model);
    pub ArrayList<Node*> nodesList{};
    pub ArrayList<Mesh*> meshesList{};

    // These are one for each unit.
    pub ListDL<ModelInstance> instancesList{LOC};

    pub explicit Model();

    pub Node* appendNode(Node* node);
    pub Mesh* appendMesh(Mesh* mesh);

    pub ModelInstance* createInstance();
    pub void deleteInstance(ModelInstance* modelInstance);

    pub void dispose() override;
    pub ~Model() override;
};
};
