#include "GB_Entries.h"
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/renderer/geometryManager/bucket/config/GeometryBucketConfig.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>

using namespace graphics;

GB_Entries::GB_Entries(sp<GeometryBucketConfig> config)
	: super(config)
{
	entriesList_Vertices = new ListDL<GeometryProviderInstance>();
	entriesList_Indices = new ListDL<GeometryProviderInstance>();

	entriesList_ModelTransforms = new ListDL<GeometryProviderInstance>();
	entriesList_ModelNormalTransforms = new ListDL<GeometryProviderInstance>();
}

int GB_Entries::getEntriesCount() {
	return entriesList_connectedGroups.count();
}

Entry_GeometryManagerBucket* GB_Entries::addRenderable(
	IGeometryProvider_Vertices* geometryProvider_Vertices, IGeometryProvider_Indices* geometryProvider_Indices,
	IGeometryProvider_ModelTransforms* geometryProvider_ModelTransforms,
	bool usesInsertAfterMode, Entry_GeometryManagerBucket* insertAfterGManEntry)
{
	if (isLogicTickInProgress) {
		throw LogicException(LOC);
	}

	GeometryProviderInstance* entry_vertices = geometryProvider_Vertices->getOrCreateFloatableInstance(this, *entriesList_Vertices);
	GeometryProviderInstance* entry_Indices = geometryProvider_Indices->getOrCreateFloatableInstance(this, *entriesList_Indices);
	GeometryProviderInstance* entry_ModelTransforms = nullptr;
	if (geometryProvider_ModelTransforms != nullptr) {
		entry_ModelTransforms = geometryProvider_ModelTransforms->getOrCreateFloatableInstance(this, *entriesList_ModelTransforms);
	}

	ListDL<Entry_GeometryManagerBucket>::Entry* newEntry_connectedGroups = nullptr;
	if (usesInsertAfterMode) {
		if (insertAfterGManEntry != nullptr) {
			if (insertAfterGManEntry->inGeometryBucket_listEntry == nullptr) {
				throw LogicException(LOC);
			}

			newEntry_connectedGroups = entriesList_connectedGroups.insertAfterEmplace(insertAfterGManEntry->inGeometryBucket_listEntry/*existingEntry*/, this/*args*/);
		} else {
			newEntry_connectedGroups = entriesList_connectedGroups.prependEmplace(this);
		}
	} else {
		newEntry_connectedGroups = entriesList_connectedGroups.appendEmplace(this);
	}
	newEntry_connectedGroups->data.inGeometryBucket_listEntry = newEntry_connectedGroups;

	entry_vertices->connectToGroup(&newEntry_connectedGroups->data);
	entry_Indices->connectToGroup(&newEntry_connectedGroups->data);
	if (entry_ModelTransforms != nullptr) {
		entry_ModelTransforms->connectToGroup(&newEntry_connectedGroups->data);
	}

	return &newEntry_connectedGroups->data;
}

void GB_Entries::removeEntry(Entry_GeometryManagerBucket* entry_geometryBucket) {
	if (isLogicTickInProgress || entry_geometryBucket->gpInstancesArray[IGeometryProvider_Vertices::PROVIDER_TYPE].gpInstances == nullptr || entry_geometryBucket->gpInstancesArray[IGeometryProvider_Indices::PROVIDER_TYPE].gpInstances == nullptr) {
		// If indices are made optional, remove the indices provider nullptr check.
		throw LogicException(LOC);
	}

	for (int i = 0; i < GEOMETRY_PROVIDER_TYPES_COUNT; i++) {
		if (entry_geometryBucket->gpInstancesArray[i].gpInstances != nullptr) {
			entry_geometryBucket->gpInstancesArray[i].gpInstances->disconnectFromGroup(entry_geometryBucket);
			entry_geometryBucket->gpInstancesArray[i].gpInstances = nullptr;
		}
	}

	// No need to use a listEntry because this is is permanent and the way this is processed.
	toRemove_entriesList.appendDirect(entry_geometryBucket);
}

void GB_Entries::removeGeometryProviderInstance(GeometryProviderInstance* providerInstance) {
	if (isLogicTickInProgress || providerInstance->connectedGMGroupsList.count() > 0) {
		throw LogicException(LOC);
	}

	toRemove_GPInstancesList[providerInstance->getProviderType()].appendDirect(providerInstance);

	// Extract the instance from the provider and save it in the entries to remove list.
	providerInstance->provider->disconnectFloatingInstance(providerInstance);
}

void GB_Entries::dispose() {
	if (entriesList_Vertices != nullptr) {
		delete entriesList_Vertices;
		entriesList_Vertices = nullptr;
	}

	if (entriesList_Indices != nullptr) {
		delete entriesList_Indices;
		entriesList_Indices = nullptr;
	}

	if (entriesList_ModelTransforms != nullptr) {
		delete entriesList_ModelTransforms;
		entriesList_ModelTransforms = nullptr;
	}

	if (entriesList_ModelNormalTransforms != nullptr) {
		delete entriesList_ModelNormalTransforms;
		entriesList_ModelNormalTransforms = nullptr;
	}

	super::dispose();
}

GB_Entries::~GB_Entries() {
	//void
}
