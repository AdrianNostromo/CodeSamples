#pragma once

#include <base/gh.h>
#include "IObject.h"

namespace base {
class Object : virtual public IObject {pub dCtor(Object);
    priv bool isDisposed = false;
    priv int isDisposedCallBitMap = 0;

    pub explicit Object();

    pub bool getIsDisposed() final;

    prot virtual void disposeMainPre();
    prot virtual void disposeMain();
    prot virtual void dispose();
    prot virtual void disposePost();

    // Note. call only one.
    pub void reservedDisposeIfNeeded() final;
    pub void reservedDispose() final;
    pub void reservedDisposeMain() final;
    pub ~Object() override;
};
};
