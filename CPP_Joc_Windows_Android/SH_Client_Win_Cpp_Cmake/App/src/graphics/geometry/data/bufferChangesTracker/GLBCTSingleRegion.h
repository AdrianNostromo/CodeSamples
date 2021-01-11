#pragma once

#include <base/gh.h>
#include "GLBCTBase.h"
#include <base/math/Bounds1DInt.h>

namespace graphics {
class GLBCTSingleRegion : public GLBCTBase {priv typedef GLBCTBase super;pub dCtor(GLBCTSingleRegion);
    priv Bounds1DInt invalidEntriesRange{};

    pub explicit GLBCTSingleRegion(CompositesBuffer* buffer);

    //asd_01;// make sure this is cleared when the on_load_to_gl upload occurs;

    pub void validate(GLenum glBindTarget) final;

    pub void invalidateUsedRegion() final;
    pub void invalidateRegion(int blockEntriesOffset, int blockEntriesCount) final;

    pub ~GLBCTSingleRegion() override;
};
};
