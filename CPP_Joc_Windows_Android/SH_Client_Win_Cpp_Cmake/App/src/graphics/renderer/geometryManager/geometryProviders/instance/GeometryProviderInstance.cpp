#include "GeometryProviderInstance.h"
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/geometryManager/bucket/IGeometryBucket.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/InCombinedRenderableBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Vertices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Indices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>

using namespace graphics;

GeometryProviderInstance::GeometryProviderInstance(IGeometryBucket* geometryBucket, IGeometryProvider* provider, void* customProviderRawPtr)
	: geometryBucket(geometryBucket),
	provider(provider), providerType(provider->getProviderType()), customProviderRawPtr(customProviderRawPtr)
{
	if (providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
		inCombinedRenderableBlockIndexedArray = new ArrayList<InCombinedRenderableBlock*>();
	} else if (providerType == IGeometryProvider_Vertices::PROVIDER_TYPE || providerType == IGeometryProvider_Indices::PROVIDER_TYPE) {
		//void
	} else {
		throw LogicException(LOC);
	}
}

int GeometryProviderInstance::getProviderType() {
	return providerType;
}

void GeometryProviderInstance::connectToGroup(Entry_GeometryManagerBucket* providersGroup) {
	int providerType = provider->getProviderType();

	if (providersGroup->gpInstancesArray[providerType].gpInstances != nullptr || providersGroup->gpInstancesArray[providerType].inGPInstance_listEntry != nullptr) {
		throw LogicException(LOC);
	}

	providersGroup->gpInstancesArray[providerType].gpInstances = this;
	providersGroup->gpInstancesArray[providerType].inGPInstance_listEntry = connectedGMGroupsList.appendDirect(providersGroup);
}

void GeometryProviderInstance::disconnectFromGroup(Entry_GeometryManagerBucket* providersGroup) {
	int providerType = provider->getProviderType();

	if (providersGroup->gpInstancesArray[providerType].gpInstances != this || providersGroup->gpInstancesArray[providerType].inGPInstance_listEntry == nullptr) {
		throw LogicException(LOC);
	}

	// Disconnect from the group.
	providersGroup->gpInstancesArray[providerType].inGPInstance_listEntry->remove();
	providersGroup->gpInstancesArray[providerType].gpInstances = nullptr;

	if (connectedGMGroupsList.count() == 0) {
		geometryBucket->removeGeometryProviderInstance(this);
	}
}

void GeometryProviderInstance::invalidate(int invalidationBitMask) {
	if (this->invalidationBitMask != 0) {
		this->invalidationBitMask |= invalidationBitMask;

		return;
	}

	geometryBucket->invalidate(this, invalidationBitMask);
}

GeometryProviderInstance::~GeometryProviderInstance() {
	if (inCombinedRenderableBlockIndexedArray != nullptr) {
		// If there is a used entry, an error occured.
		for (int i = 0; i < inCombinedRenderableBlockIndexedArray->count(); i++) {
			InCombinedRenderableBlock* entry = inCombinedRenderableBlockIndexedArray->getDirect(i);
			if (entry != nullptr) {
				delete entry;
			}
		}

		delete inCombinedRenderableBlockIndexedArray;
		inCombinedRenderableBlockIndexedArray = nullptr;
	}
}
