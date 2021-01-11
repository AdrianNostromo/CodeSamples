#include "GM_Buckets.h"
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/renderPhase/IRenderPhase.h>
#include <graphics/renderer/geometryManager/bucket/util/GeometryBucketUser.h>
#include <graphics/renderer/geometryManager/bucket/GeometryBucket.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/renderer/geometryManager/bucket/config/GeometryBucketConfig.h>
#include <graphics/renderer/renderablesManager/util/Entry_RenderablesManager.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>

using namespace graphics;

GM_Buckets::GM_Buckets()
	: super()
{
	//void
}

void GM_Buckets::createMain() {
	super::createMain();

	//asd_01;// vaList is not available at start, maybe create the buffers when the first entry is added.

	//vbo = new VertexBufferObject(vertexAttributesList, 30000/*initialCapacity*/, 30000/*minCapacity*/);
	//vbo->getBuffer().lockSize();
	//vbo->reservedCreate();
	//vaStride = vertexAttributesList->getStride();

	//ibo = new IndexBufferObject(maxIndicesCount/*initialCapacity*/, maxIndicesCount/*minCapacity*/)
	//ibo->getBuffer().lockSize();
	//ibo->reservedCreate();

	//asd_01;// the transforms UBO if optional, use the first entry add to deduce if transformArray or no_transform is needed;
}

void GM_Buckets::onRenderPhaseAdded(IRenderPhase* renderPhase) {
	if (isAnyRenderableAdded) {
		//asd_x; Currently this must be done before any renderables are added. When needed to have optional rPhases, enum all renderables and add them to the new buckets as needed.
		throw LogicException(LOC);
	}

	// Hook the buckets from the rPhase.
	for (ListDL<GeometryBucketUser>::Entry* entry = renderPhase->getGeometryBucketUsers().getFirst(); entry != nullptr; entry = entry->next) {
		GeometryBucketUser* bucketUser = &entry->data;

		if (bucketUser->bucket != nullptr || bucketUser->inBucket_listEntry != nullptr) {
			throw LogicException(LOC);
		}

		GeometryBucket* existingBucket = getBucketWithConfig(*bucketUser->config.get());
		if (existingBucket == nullptr) {
			// Create a new bucket.
			ListDL<GeometryBucket>::Entry* newEntry = geometryBucketsList.appendEmplace(bucketUser->config);
			existingBucket = &newEntry->data;
			existingBucket->inGeometryManager_listEntry = newEntry;

			existingBucket->reservedCreate();
		}

		bucketUser->bucket = existingBucket;
		bucketUser->inBucket_listEntry = existingBucket->usersList.appendDirect(bucketUser);
	}
}

void GM_Buckets::onRenderPhaseRemovePre(IRenderPhase* renderPhase) {
	// Release the buckets used by the rPhase. The buckets may be removed if not used by other rPhases anymore.
	for (ListDL<GeometryBucketUser>::Entry* entry = renderPhase->getGeometryBucketUsers().getFirst(); entry != nullptr; entry = entry->next) {
		GeometryBucketUser* bucketUser = &entry->data;

		if (bucketUser->bucket == nullptr || bucketUser->inBucket_listEntry == nullptr) {
			throw LogicException(LOC);
		}

		GeometryBucket* existingBucket = static_cast<GeometryBucket*>(bucketUser->bucket->rawSubclassPointer);

		bucketUser->bucket = nullptr;
		bucketUser->inBucket_listEntry->remove();
		bucketUser->inBucket_listEntry = nullptr;

		if (existingBucket->usersList.count() == 0) {
			// Bucket is no longer used, remove it.

			// This should remove any pending removal entries.
			existingBucket->prepareForRemoval();

			if (existingBucket->getEntriesCount() > 0) {
				//asd_x; Currently this must be done before any renderables are added. When needed to have optional rPhases, remove all entries from the buckets that will be removed.
				throw LogicException(LOC);
			}

			existingBucket->reservedDispose();

			existingBucket->inGeometryManager_listEntry->remove();
			//The memory got freed in the above call.
			//existingBucket->inGeometryManager_listEntry = nullptr;
		}
	}
}

void GM_Buckets::addRenderable(
	IRenderable* renderable, ListDL<Entry_GeometryManagerBucket*>& gManagerBucketEntriesList,
	bool usesInsertAfterMode, IRenderable* insertAfterNodePartTarget)
{
	isAnyRenderableAdded = true;

	std::shared_ptr<VertexAttributesList> vertexAttributes = renderable->getGeometryProvider_Vertices()->getVertexAttributesList();
	std::shared_ptr<graphics::Material> material = renderable->getGeometryProvider_Vertices()->getMaterial();

	// Enumerate all buckets and create a Entry_GeometryManagerBucket for each bucket use.
	bool isAddedToAnyBucket = false;
	for (ListDL<GeometryBucket>::Entry* entry = geometryBucketsList.getFirst(); entry != nullptr; entry = entry->next) {
		GeometryBucket* gBucket = &entry->data;
		// Note. Do the can handle test here because the insertAfterTarget needs some processing and IRenderable should not be passed to the gBucket.
		if (!gBucket->config->equals(vertexAttributes, material)) {
			continue;
		}

		// Check that the insert after target is in this bucket (that is a requirement).
		Entry_GeometryManagerBucket* insertAfterGManEntry = nullptr;
		if (usesInsertAfterMode && insertAfterNodePartTarget != nullptr) {
			if (insertAfterNodePartTarget->getEntry_renderablesManagerRef()->gManagerBucketEntriesList.count() != 1) {
				throw LogicException(LOC);
			}

			insertAfterGManEntry = insertAfterNodePartTarget->getEntry_renderablesManagerRef()->gManagerBucketEntriesList.getFirst()->data;
			if (insertAfterGManEntry == nullptr || insertAfterGManEntry->geometryBucket != gBucket) {
				// Missing insert after target value or the insertAfterTarget is in another bucket (logic works if it is in this bucket only).
				throw LogicException(LOC);
			}
		}

		Entry_GeometryManagerBucket* newEntry = gBucket->addRenderable(
			renderable->getGeometryProvider_Vertices(), renderable->getGeometryProvider_Indices(),
			renderable->getGeometryProvider_ModelTransforms(),
			usesInsertAfterMode, insertAfterGManEntry
		);
		if (newEntry == nullptr) {
			throw LogicException(LOC);
		}

		gManagerBucketEntriesList.appendDirect(newEntry);

		isAddedToAnyBucket = true;
	}

	if (!isAddedToAnyBucket) {
		// The renderable is not handled.
		throw LogicException(LOC);
	}
}

GeometryBucket* GM_Buckets::getBucketWithConfig(GeometryBucketConfig& bucketConfig) {
	for (ListDL<GeometryBucket>::Entry* entry = geometryBucketsList.getFirst(); entry != nullptr; entry = entry->next) {
		if (entry->data.config->equals(bucketConfig)) {
			return &entry->data;
		}

	}

	return nullptr;
}

void GM_Buckets::dispose() {
	if (geometryBucketsList.count() > 0) {
		// The buckets have to be removed before this is disposed.
		throw LogicException(LOC);
	}

	super::dispose();
}

GM_Buckets::~GM_Buckets() {
	//void
}
