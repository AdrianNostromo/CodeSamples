#include "Object.h"
#include <base/exceptions/LogicException.h>
#include <cassert>

using namespace base;

Object::Object() {
    //void
}

bool Object::getIsDisposed() {
    return isDisposed;
}

void Object::reservedDisposeMain() {
    reservedDispose();
}

void Object::reservedDisposeIfNeeded() {
    if(isDisposed) {
        return;
    }

    reservedDispose();
}

void Object::reservedDispose() {
    if(isDisposed) {
        throw LogicException(LOC);
    }

    isDisposedCallBitMap = 0;
    disposeMainPre();
    disposeMain();
    dispose();
    disposePost();
    if (isDisposedCallBitMap != (1 | 2 | 4 | 8)) {
        throw LogicException(LOC);
    }

    isDisposed = true;
}

void Object::disposeMainPre() {
    if ((isDisposedCallBitMap & 1) != 0) {
        throw LogicException(LOC);
    }
    isDisposedCallBitMap |= 1;
}

void Object::disposeMain() {
    if ((isDisposedCallBitMap & 2) != 0) {
        throw LogicException(LOC);
    }
    isDisposedCallBitMap |= 2;
}

void Object::dispose() {
    if ((isDisposedCallBitMap & 4) != 0) {
        throw LogicException(LOC);
    }
    isDisposedCallBitMap |= 4;
}

void Object::disposePost() {
    if ((isDisposedCallBitMap & 8) != 0) {
        throw LogicException(LOC);
    }
    isDisposedCallBitMap |= 8;
}

Object::~Object() {
    if(!isDisposed) {
        // Must call a reservedDispose function manually because calling it from here doesn't work.
        // Calling virtual extended functions from a base class destructor doesn't work.
        // After calling a class layer destructor (eg. the subclasses of this), the virtual functions mechanism is disabled for that.
        // Calling a virtual class (eg. dispose()) will not call it's overriders.
        // As a workaround, need to call reservedDisposeIfNeeded() from the most subclass destructor when auto destruction is enabled.
        assert(false);
    }

    //void
}
