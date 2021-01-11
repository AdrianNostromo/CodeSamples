#include "GB_CombinedRenderables.h"
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <base/opengl/util/GLUtil.h>
#include <base/opengl/IGL.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>

using namespace graphics;

GB_CombinedRenderables::GB_CombinedRenderables(sp<GeometryBucketConfig> config)
	: super(config)
{
	//void
}

ListDL<CombinedRenderable>& GB_CombinedRenderables::getCombinedRenderablesList() {
	return combinedRenderablesList;
}

CombinedRenderable* GB_CombinedRenderables::getNextCombinedRenderable(
	CombinedRenderable* currentCombinedRenderable,
	GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices,
	GeometryProviderInstance* gpi_ModelTransforms)
{
	if (currentCombinedRenderable == nullptr && combinedRenderablesList.count() > 0) {
		return &combinedRenderablesList.getFirst()->data;
	} else if (currentCombinedRenderable != nullptr && currentCombinedRenderable->inGeometryManager_listEntry->next != nullptr) {
		return &currentCombinedRenderable->inGeometryManager_listEntry->next->data;
	}

	// Create a new entry.
	if (combinedRenderablesList.count() > 50) {
		// There should never be so many CombinedRenderables active. A logic error probably occured.
		throw LogicException(LOC);
	}
	if (!static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVertexAttributesList()->getIsLocked()) {
		// The vertexAttributesList must be locked.
		throw LogicException(LOC);
	}

	std::shared_ptr<graphics::Material> material = static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getMaterial();

	// The indices limit is set higher because the vertices count is the limit that needs to be under u_short max value.
	/// Because most of the geometry is disconnected quads for now, the indices_count=vertices_count/4*6; This should be a optimal fill.
	/// Because vertices occupy much more space, it is preffered the vertices to be the insert_limiting_factor. That is why the indices have a extra ~20% size increase.
	// The transforms count can be increased but with a const value for the app run or it will cause a new shader compile (the count value is used as define in the shader array size).
	int maxVerticesCount = Math::min(65536, GLUtil::gl->getCapabilities().maxElementVertices);
	int maxIndicesCount = Math::min((int)(((maxVerticesCount / 4.0f) * 6.0f) * 1.2f), GLUtil::gl->getCapabilities().maxElementIndices);
	int maxWorldTransformsCount = Math::min(1000/*preferred value*/, GLUtil::gl->getCapabilities().maxUniformBlockSizeBytes / (4/*rows*/ * 4/*columns*/ * 4/*bytes=1_float*/));
	ListDL<CombinedRenderable>::Entry* entry = combinedRenderablesList.appendEmplace(
		static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVertexAttributesList(),
		material,
		maxVerticesCount/*maxVerticesCount*/, maxIndicesCount/*maxIndicesCount*/, maxWorldTransformsCount/*maxWorldTransformsCount*/
	);
	CombinedRenderable* combinedRenderable = &entry->data;
	combinedRenderable->inGeometryManager_listEntry = entry;
	combinedRenderable->reservedCreate();

	// Set a new uIndex. This is required here to avoid cRenderables with high uIndex value that will cause the indexed arrays that use it to have larger than needed size.
	combinedRenderable->uIndex = getNextAvailableCombinedRenderableUIndex();

	if (combinedRenderablesList_usedUIndicesTracker.getDirect(combinedRenderable->uIndex, 0/*fallbackValue*/) != 0) {
		// Entry already exists.
		throw LogicException(LOC);
	}
	combinedRenderablesList_usedUIndicesTracker.insertDirectRangeFill(combinedRenderable->uIndex, 1/*val*/, 0/*rangeFillVal*/);

	return combinedRenderable;
}

int GB_CombinedRenderables::getNextAvailableCombinedRenderableUIndex() {
	for (int i = 0; i < combinedRenderablesList_usedUIndicesTracker.count(); i++) {
		if (combinedRenderablesList_usedUIndicesTracker.getDirect(i) == 0) {
			return i;
		}
	}

	return combinedRenderablesList_usedUIndicesTracker.count();
}

void GB_CombinedRenderables::dispose() {
	while (combinedRenderablesList.count() > 0) {
		ListDL<CombinedRenderable>::Entry* entry = combinedRenderablesList.getFirst();

		entry->data.reservedDispose();

		entry->remove();
	}

	super::dispose();
}

GB_CombinedRenderables::~GB_CombinedRenderables() {
	//void
}
