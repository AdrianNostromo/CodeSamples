#include "GeometryBucketConfig.h"

using namespace graphics;

GeometryBucketConfig::GeometryBucketConfig(std::shared_ptr<VertexAttributesList> vertexAttributesList, std::shared_ptr<graphics::Material> material)
	: vertexAttributesList(vertexAttributesList), material(material)
{
	//void
}

bool GeometryBucketConfig::equals(GeometryBucketConfig& other) {
	if (this->vertexAttributesList != other.vertexAttributesList || this->material != other.material) {
		return false;
	}

	return true;
}

bool GeometryBucketConfig::equals(std::shared_ptr<VertexAttributesList> vertexAttributesList, std::shared_ptr<graphics::Material> material) {
	if (this->vertexAttributesList != vertexAttributesList || this->material != material) {
		return false;
	}

	return true;
}
