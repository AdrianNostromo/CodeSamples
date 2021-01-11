#pragma once

#include <base/gh.h>
#include "IGLBufferChangesTracker.h"

class CompositesBuffer;

namespace graphics {
class GLBCTBase : public virtual IGLBufferChangesTracker {pub dCtor(GLBCTBase);
    prot CompositesBuffer* buffer;

    pub explicit GLBCTBase(CompositesBuffer* buffer);

    pub ~GLBCTBase() override;
};
};
