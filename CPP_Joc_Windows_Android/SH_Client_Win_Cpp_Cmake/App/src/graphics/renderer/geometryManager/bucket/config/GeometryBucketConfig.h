#pragma once

#include <base/gh.h>
#include <memory>

namespace graphics {
    class VertexAttributesList;
    class Material;
};

namespace graphics {
class GeometryBucketConfig {pub dCtor(GeometryBucketConfig);
    // Currently, these must be the values from GraphicsUtil and they are instance comparted (==) directly.
    pub std::shared_ptr<VertexAttributesList> vertexAttributesList;
    pub std::shared_ptr<graphics::Material> material;

    pub explicit GeometryBucketConfig(std::shared_ptr<VertexAttributesList> vertexAttributesList, std::shared_ptr<graphics::Material> material);

    pub bool equals(GeometryBucketConfig& other);
    pub bool equals(std::shared_ptr<VertexAttributesList> vertexAttributesList, std::shared_ptr<graphics::Material> material);

};
};
