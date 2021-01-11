#include "GM_Validation.h"
#include <graphics/renderer/geometryManager/combinedRenderable/CombinedRenderable.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <graphics/material/attributeTypes/MABlending.h>
#include <graphics/shader/Shader.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <memory>
#include <base/opengl/IGL.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/InCombinedRenderableBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>
#include <graphics/renderer/geometryManager/bucket/GeometryBucket.h>

using namespace graphics;

GM_Validation::GM_Validation()
	: super()
{
	//void
}

void GM_Validation::tickLogic() {
	for (ListDL<GeometryBucket>::Entry* entry = geometryBucketsList.getFirst(); entry != nullptr; entry = entry->next) {
		entry->data.tickLogic();
	}
}

GM_Validation::~GM_Validation() {
	//void
}
