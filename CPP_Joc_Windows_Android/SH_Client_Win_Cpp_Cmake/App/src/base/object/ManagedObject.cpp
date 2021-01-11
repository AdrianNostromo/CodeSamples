#include "ManagedObject.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

using namespace base;

ManagedObject::ManagedObject()
    : super()
{
    //void
}

void ManagedObject::reservedCreate() {
    if(isCreated) {
        throw LogicException(LOC);
    }
    isCreated = true;

    isCreatedCallBitMap = 0;
    createMain();
    create();
    createPost();
    if (isCreatedCallBitMap != (1 | 2 | 4)) {
        throw LogicException(LOC);
    }
}

bool ManagedObject::getIsCreated() {
    return isCreated;
}

void ManagedObject::createMain() {
    if ((isCreatedCallBitMap & 1) != 0) {
        throw LogicException(LOC);
    }
    isCreatedCallBitMap |= 1;
}

void ManagedObject::create() {
    if ((isCreatedCallBitMap & 2) != 0) {
        throw LogicException(LOC);
    }
    isCreatedCallBitMap |= 2;
}

void ManagedObject::createPost() {
    if ((isCreatedCallBitMap & 4) != 0) {
        throw LogicException(LOC);
    }
    isCreatedCallBitMap |= 4;
}

ManagedObject::~ManagedObject() {
    // Add this check to catch some odd bugs.
    if (!isCreated) {
        assert(false);
    }
}
