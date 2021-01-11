#pragma once

#include <base/gh.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/defragmenter/IDefragmenterBlockMemory.h>

namespace graphics {
class Defrag_Base : public virtual IDefragmenterBlockMemory {pub dCtor(Defrag_Base);
    pub explicit Defrag_Base();

    pub ~Defrag_Base() override;
};
};
