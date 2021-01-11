#include "IndexBufferObject.h"
#include <assert.h>
#include <base/opengl/util/GLUtil.h>
#include <iostream>

using namespace graphics;

IndexBufferObject::IndexBufferObject(
    UpdateMethod updateMethod,
    int initialCapacity, int minCapacity)
    : super(
        &BindLocations::ELEMENT_ARRAY_BUFFER/*bindLocationIndex*/, IGL::ELEMENT_ARRAY_BUFFER/*glBindTarget*/,
        sizeof(unsigned short)/*stride*/,
        updateMethod,
        initialCapacity/*initialCapacity*/, minCapacity/*minCapacity*/
    )
{
    rawSubclassPointer = this;

    //void
}

int IndexBufferObject::getActiveIndicesCount() {
    return activeIndicesCount;
}

void IndexBufferObject::setActiveIndicesCount(int activeIndicesCount) {
    this->activeIndicesCount = activeIndicesCount;
}

void IndexBufferObject::disposeMain() {
    //void

    super::disposeMain();
}

IndexBufferObject::~IndexBufferObject() {
    //void
}
