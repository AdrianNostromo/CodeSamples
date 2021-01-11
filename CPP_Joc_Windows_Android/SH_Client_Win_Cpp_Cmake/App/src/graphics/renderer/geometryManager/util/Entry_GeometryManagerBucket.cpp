#include "Entry_GeometryManagerBucket.h"
#include <graphics/renderer/geometryManager/IGeometryManager.h>
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/InCombinedRenderableBlock.h>
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Vertices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Indices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>
#include <graphics/renderer/geometryManager/bucket/IGeometryBucket.h>
#include "../combinedRenderable/ICombinedRenderable.h"
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>

using namespace graphics;

Entry_GeometryManagerBucket::Entry_GeometryManagerBucket(IGeometryBucket* geometryBucket)
	: geometryBucket(geometryBucket)
{
	//void
}

void Entry_GeometryManagerBucket::remove() {
	if (geometryBucket == nullptr) {
		throw LogicException(LOC);
	}

	geometryBucket->removeEntry(this);
}

InCombinedRenderableBlock* Entry_GeometryManagerBucket::getInCombinedRenderableBlockIfExists(int providerType, int combinedRenderableUIndex) {
	InCombinedRenderableBlock* entry = nullptr;
	if (providerType == IGeometryProvider_Vertices::PROVIDER_TYPE || providerType == IGeometryProvider_Indices::PROVIDER_TYPE) {
		entry = inCombinedRenderableProviderBlocks[providerType];
	} else if (providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
		ProviderInstanceEntry& gpiWrapper = gpInstancesArray[providerType];
		if (gpiWrapper.gpInstances == nullptr) {
			throw LogicException(LOC);
		}

		entry = gpiWrapper.gpInstances->inCombinedRenderableBlockIndexedArray->getDirect(combinedRenderableUIndex, nullptr);
	} else {
		throw LogicException(LOC);
	}

	return entry;
}

InCombinedRenderableBlock* Entry_GeometryManagerBucket::getInCombinedRenderableBlockIfUsed(int providerType, int combinedRenderableUIndex) {
	InCombinedRenderableBlock* entry = getInCombinedRenderableBlockIfExists(providerType, combinedRenderableUIndex);
	if (entry != nullptr && entry->memBlock != nullptr && entry->memBlock->memMan->combinedRenderable->uIndex == combinedRenderableUIndex) {
		return entry;
	}

	return nullptr;
}

InCombinedRenderableBlock* Entry_GeometryManagerBucket::getOrCreateInCombinedRenderableBlock(int providerType, int combinedRenderableUIndex) {
	static bool IsStaticsInit = false;
	static InCombinedRenderableBlock* (*ICRB_FactoriesList[GEOMETRY_PROVIDER_TYPES_COUNT])();
	if (!IsStaticsInit) {
		IsStaticsInit = true;

		if (GEOMETRY_PROVIDER_TYPES_COUNT != 3) {
			// Update the factory array as required.
			throw LogicException(LOC);
		}

		ICRB_FactoriesList[IGeometryProvider_Vertices::PROVIDER_TYPE] = &ICRB_Vertices::GetNew;
		ICRB_FactoriesList[IGeometryProvider_Indices::PROVIDER_TYPE] = &ICRB_Indices::GetNew;
		ICRB_FactoriesList[IGeometryProvider_ModelTransforms::PROVIDER_TYPE] = &ICRB_ModelTransforms::GetNew;
	}

	if (providerType == IGeometryProvider_Vertices::PROVIDER_TYPE || providerType == IGeometryProvider_Indices::PROVIDER_TYPE) {
		InCombinedRenderableBlock* entry = inCombinedRenderableProviderBlocks[providerType];
		if (entry == nullptr) {
			entry = ICRB_FactoriesList[providerType]();
			inCombinedRenderableProviderBlocks[providerType] = entry;
		}

		return entry;
	} else if (providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
		ProviderInstanceEntry& gpiWrapper = gpInstancesArray[providerType];
		if (gpiWrapper.gpInstances == nullptr) {
			throw LogicException(LOC);
		}

		while (gpiWrapper.gpInstances->inCombinedRenderableBlockIndexedArray->count() <= combinedRenderableUIndex) {
			gpiWrapper.gpInstances->inCombinedRenderableBlockIndexedArray->appendDirect(nullptr);
		}

		InCombinedRenderableBlock* entry = gpiWrapper.gpInstances->inCombinedRenderableBlockIndexedArray->getDirect(combinedRenderableUIndex, nullptr);
		if (entry == nullptr) {
			entry = ICRB_FactoriesList[providerType]();
			gpiWrapper.gpInstances->inCombinedRenderableBlockIndexedArray->setDirect(combinedRenderableUIndex, entry);
		}

		return entry;
	} else {
		throw LogicException(LOC);
	}
}

Entry_GeometryManagerBucket::~Entry_GeometryManagerBucket() {
	for (int i = 0; i < GEOMETRY_PROVIDER_TYPES_COUNT; i++) {
		InCombinedRenderableBlock* entry = inCombinedRenderableProviderBlocks[i];
		if (entry != nullptr) {
			delete entry;
		}
	}
}
