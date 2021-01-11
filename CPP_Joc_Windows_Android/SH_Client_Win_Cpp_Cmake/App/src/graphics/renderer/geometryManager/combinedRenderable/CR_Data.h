#pragma once

#include <base/gh.h>
#include "CR_Base.h"

namespace graphics {
class CR_Data : public CR_Base {priv typedef CR_Base super;pub dCtor(CR_Data);
    
    pub explicit CR_Data(
        std::shared_ptr<VertexAttributesList> vertexAttributesList,
        std::shared_ptr<graphics::Material> material,
        int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount);

    pub ~CR_Data() override;
};
};
