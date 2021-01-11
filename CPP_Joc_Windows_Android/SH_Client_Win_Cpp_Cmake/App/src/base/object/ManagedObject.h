#pragma once

#include <base/gh.h>
#include "Object.h"
#include "IManagedObject.h"

namespace base {
class ManagedObject : public Object, virtual public IManagedObject {priv typedef Object super;pub dCtor(ManagedObject);
    priv bool isCreated = false;
    priv int isCreatedCallBitMap = 0;

    pub explicit ManagedObject();
    pub void reservedCreate() final;

    prot virtual void createMain();
    prot virtual void create();
    prot virtual void createPost();

    pub bool getIsCreated() override;

    pub ~ManagedObject() override;
};
};
