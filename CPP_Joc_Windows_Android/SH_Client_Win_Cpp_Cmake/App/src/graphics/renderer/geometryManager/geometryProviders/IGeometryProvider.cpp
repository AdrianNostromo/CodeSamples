#include "IGeometryProvider.h"
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <base/exceptions/LogicException.h>
#include <cassert>
#include <base/math/util/BoolUtil.h>

using namespace graphics;

const int IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup = BoolUtil::setBitAtIndexDirect(0, 0);
const int IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup = BoolUtil::setBitAtIndexDirect(0, 1);

const int IGeometryProvider::InvalidationBitGroups::GROUPS_COUNT = 2;

int IGeometryProvider::GetNew_PROVIDER_TYPE() {
	static int COUNTER = 0;

	int ret = COUNTER;
	if (ret >= GEOMETRY_PROVIDER_TYPES_COUNT) {
		// There are more provider types requested than the hardcoded amount. Need to update some arrays that use GEOMETRY_PROVIDER_TYPES_COUNT for size initialisation.
		assert(false);
	}

	COUNTER++;

	return ret;
}

GeometryProviderInstance* IGeometryProvider::getOrCreateFloatableInstance(IGeometryBucket* geometryBucket, ListDL<GeometryProviderInstance>& entriesListManaged_Vertices) {
	for (ListDL<GeometryProviderInstance*>::Entry* entry = instancesListUnmanaged.getFirst(); entry != nullptr; entry = entry->next) {
		if (entry->data->geometryBucket == geometryBucket) {
			return entry->data;
		}
	}

	// The instance is managed by the gManager, not by "this" because it can be disconnected and left floating when it will be disposed.
	ListDL<GeometryProviderInstance>::Entry* newInstanceEntry = entriesListManaged_Vertices.appendEmplace(geometryBucket, this, getProviderInterfaceVoidPointer());
	GeometryProviderInstance* provider = &newInstanceEntry->data;

	provider->inGeometryBucket_listEntry = newInstanceEntry;

	// Connect the new instance to this provider.
	provider->inProvider_listEntry = instancesListUnmanaged.appendDirect(provider);

	return provider;
}

void IGeometryProvider::disconnectFloatingInstance(GeometryProviderInstance* instance) {
	if (instance == nullptr || instance->provider != this || instance->inProvider_listEntry == nullptr || instance->geometryBucket == nullptr || instance->inGeometryBucket_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	instance->provider = nullptr;

	instance->inProvider_listEntry->remove();
	instance->inProvider_listEntry = nullptr;
}

void IGeometryProvider::invalidate(int invalidationBitMask) {
	for (ListDL<GeometryProviderInstance*>::Entry* entry = instancesListUnmanaged.getFirst(); entry != nullptr; entry = entry->next) {
		entry->data->invalidate(invalidationBitMask);
	}
}

IGeometryProvider::~IGeometryProvider() {
	if (instancesListUnmanaged.count() > 0) {
		// All instances must be removed before this gets disposed.
		assert(false);
	}
}
