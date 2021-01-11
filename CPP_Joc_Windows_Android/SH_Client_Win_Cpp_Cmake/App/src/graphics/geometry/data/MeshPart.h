#pragma once

#include <base/gh.h>
#include <base/object/Object.h>

namespace graphics {
    class Mesh;
};

namespace graphics {
class MeshPart final/*Note1001. calls_the_reserved_dispose*/ : public base::Object {priv typedef base::Object super;pub dCtor(MeshPart);
    pub Mesh* mesh;

    // These are for indices.
    pub unsigned int verticesOffset;
    pub unsigned verticesCount;
    
    pub unsigned int indicesOffset;
    pub unsigned indicesCount;
    
    pub explicit MeshPart(
        Mesh* mesh, 
        unsigned int verticesOffset, unsigned verticesCount,
        unsigned int indicesOffset, unsigned indicesCount);

    pub ~MeshPart() override;
};
};
