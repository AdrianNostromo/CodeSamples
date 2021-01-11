#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/math/Matrix4.h>
#include <base/list/ArrayList.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

class IListEntry;
namespace graphics {
    class Model;
    class NodeInstance;
};

namespace graphics {
class ModelInstance : public base::Object {priv typedef base::Object super;pub dCtor(ModelInstance);
    pub Model* model;

    // Convert to a ListDL.
    pub ArrayList<NodeInstance*> nodesList{};

    pub unsigned int invalidationBitMap = -1;

    priv Vector3 translation{};
    priv Quaternion rotation{};
    priv Vector3 scale{1.0f, 1.0f, 1.0f};

    priv Matrix4 localTransform{};
    pub Matrix4 globalTransform{};

    // These are used for ModelInstance-s rendering.
    pub IListEntry* inRenderer_listEntry = nullptr;

    pub IListEntry* inModel_listEntry = nullptr;

    pub explicit ModelInstance(Model* model);

    pub Matrix4& peekGlobalTransform();
    
    pub void invalidate(unsigned int invalidationBitMap);
    pub void sync(unsigned int upperInvalidationBitMap);

    pub void setTranslation(float x, float y, float z);
    pub void setRotation(Quaternion& rotation);
    pub void setScale(float scaleX, float scaleY, float scaleZ);

    prot void dispose() override;
    pub ~ModelInstance() override;
};
};
