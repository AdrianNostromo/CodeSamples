#pragma once

#include <base/gh.h>
#include "BufferObject.h"
#include <base/list/CompositesBuffer.h>

namespace graphics {
class IndexBufferObject : public BufferObject {priv typedef BufferObject super;pub dCtor(IndexBufferObject);
    // unsigned short
    
    // asd_r;// remove when new renderer method is implemented and this is no longer used.
    // If >=0 overrides the rendered indices count.
    //asd_r;// use the buffer count instead of this.
    priv int activeIndicesCount = -1;

    pub explicit IndexBufferObject(
        UpdateMethod updateMethod,
        int initialCapacity = 0, int minCapacity = 0);

    //asd_r;// remove when new renderer method is implemented and this is no longer used.
    pub int getActiveIndicesCount();
    pub void setActiveIndicesCount(int activeIndicesCount);

    prot void disposeMain() override;
    pub ~IndexBufferObject() override;
};
};
