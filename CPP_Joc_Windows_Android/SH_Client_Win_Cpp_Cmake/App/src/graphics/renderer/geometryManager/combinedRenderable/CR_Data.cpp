#include "CR_Data.h"

using namespace graphics;

CR_Data::CR_Data(
	std::shared_ptr<VertexAttributesList> vertexAttributesList,
	std::shared_ptr<graphics::Material> material,
	int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount)
	: super(vertexAttributesList,
		material,
		maxVerticesCount, maxIndicesCount, maxWorldTransformsCount)
{
	//void
}

CR_Data::~CR_Data() {
	//void
}
