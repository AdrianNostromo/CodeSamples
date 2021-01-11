#pragma once

#include <base/gh.h>
#include <base/object/IManagedObject.h>
#include <base/list/ListDL.h>

class IListEntry;
namespace graphics {
    class IEnvironment;
    class IRenderer;
    class IGeometryManager;
    class IRenderable;
    class Entry_GeometryManagerBucket;
    class GeometryBucketUser;
};

namespace graphics {
class IRenderPhase : public virtual base::IManagedObject {priv typedef base::IManagedObject super;
    prot static int GetNew_TYPE();
    pub virtual int getType() = 0;

    prot void* rawSubclassPointer = nullptr;

    pub virtual int getRenderOrder() = 0;

    pub virtual bool getIsEnabled() = 0;
    pub virtual void setIsEnabled(bool isEnabled) = 0;

    pub virtual IListEntry*& getInHandlerListEntryRef() = 0;
    pub virtual IRenderer* getRenderer() = 0;
    pub virtual void setRenderer(IRenderer* renderer) = 0;

    pub virtual void render(graphics::IEnvironment* environment) = 0;

    pub virtual ListDL<GeometryBucketUser>& getGeometryBucketUsers() = 0;

    pub template <typename T>
    T castAs(int validationType);

    pub virtual ~IRenderPhase() = default;
};

template <typename T>
T IRenderPhase::castAs(int validationType) {
    if (this->getType() != validationType || rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
