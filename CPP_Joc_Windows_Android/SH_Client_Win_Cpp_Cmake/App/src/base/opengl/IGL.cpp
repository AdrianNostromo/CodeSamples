#include "IGL.h"
#include <base/exceptions/LogicException.h>

IGL::CbListener::CbListener()
    : super()
{
    //void
}

void IGL::CbListener::onGlContextCreated() {
    if(cb_onGlContextCreated) {
        cb_onGlContextCreated();
    }
}

void IGL::CbListener::onGlContextDestroyedPre() {
    if(cb_onGlContextDestroyedPre) {
        cb_onGlContextDestroyedPre();
    }
}

IGL::CbListener::~CbListener() {
    //void
}

unsigned int IGL::sizeOfType(GLuint type) {
    if(type == BYTE) { return 1; }
    else if(type == UNSIGNED_BYTE) { return 1; }
    else if(type == SHORT) { return 2; }
    else if(type == UNSIGNED_SHORT) { return 2; }
    else if(type == INT) { return 4; }
    else if(type == UNSIGNED_INT) { return 4; }
    else if(type == FIXED) { return 4; }
    else if(type == FLOAT) { return 4; }

    throw LogicException(LOC);
}
