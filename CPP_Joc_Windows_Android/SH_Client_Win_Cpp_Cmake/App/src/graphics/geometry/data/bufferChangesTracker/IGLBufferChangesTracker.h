#pragma once

#include <base/gh.h>
#include <base/opengl/IGL.h>

namespace graphics {
class IGLBufferChangesTracker {
    pub virtual void validate(GLenum glBindTarget) = 0;

    pub virtual void invalidateUsedRegion() = 0;
    pub virtual void invalidateRegion(int blockEntriesOffset, int blockEntriesCount) = 0;

    pub virtual ~IGLBufferChangesTracker() = default;
};
};
