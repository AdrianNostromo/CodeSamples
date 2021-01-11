#include "GB_Validation.h"
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Vertices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Indices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include "config/GeometryBucketConfig.h"

using namespace graphics;

float GB_Validation::PreferredCrendFillRate = 0.9f;
float GB_Validation::NextCRendJumpIfAlreadyInItFillRate = 0.85f;

GB_Validation::GB_Validation(sp<GeometryBucketConfig> config)
	: super(config)
{
	//void
}

void GB_Validation::invalidate(GeometryProviderInstance* gpInstance, int invalidationBitMask) {
	if (gpInstance->invalidationBitMask != 0 || gpInstance->inGMan_invalidation_listEntry != nullptr) {
		// This invalidate function should be called only on first invalidation. On recuring invalidations just update the entry invalidation bit mask.
		throw LogicException(LOC);
	}

	gpInstance->invalidationBitMask |= invalidationBitMask;
	gpInstance->inGMan_invalidation_listEntry = invalidGPInstancesListsArray.getReference(gpInstance->getProviderType()).appendDirect(gpInstance);
}

void GB_Validation::tickLogic() {
	if (isLogicTickInProgress) {
		throw LogicException(LOC);
	}
	isLogicTickInProgress = true;

	tickLogic_toRemoveEntries();
	tickLogic_invalidationGProviderInstances();
	tickLogic_prepareCRenderablesForEntriesEnumerationInsertionPhase();
	tickLogic_combineEntriesNoDataWrites();
	tickLogic_defragmentations();
	tickLogic_writeChangedDatas();

	// If there are empty cRenderables, remove the extra one so that only 1 empty cRenderable may exist at a time.
	/// The check also test the attributes using a list.
	/// This check will always keep at leas 1 type of each cRenderable even if that type is is never used (eg. no blended renderables exist) after it was used at least once.
	//asd_01;

	tickLogic_clearGPInstancesInvalidationFlags();
	tickLogic_handleDelayedICRModelTransformsRemoval();

	isLogicTickInProgress = false;

	//asd_01;// implement a basic defragmenter for vertices and transforms, it does full defragment each frame.

}

void GB_Validation::tickLogic_toRemoveEntries() {
	// Remove vertices and indices using toRemove_entriesList.
	while (toRemove_entriesList.count() > 0) {
		ListDL<Entry_GeometryManagerBucket*>::Entry* entry = toRemove_entriesList.getFirst();
		Entry_GeometryManagerBucket* gbEntry = entry->data;
		// This entry is just for a pointer, it can be removed now before the actual oject gets deleted.
		entry->remove();

		// The model_transforms are processed below.
		const int providerTypesToProcess_count = 2;
		int providerTypesToProcess_array[providerTypesToProcess_count] {
			IGeometryProvider_Vertices::PROVIDER_TYPE,
			IGeometryProvider_Indices::PROVIDER_TYPE
		};
		for (int i = 0; i < providerTypesToProcess_count; i++) {
			int providerType = providerTypesToProcess_array[i];

			InCombinedRenderableBlock* inCombinedRenderableBlock = gbEntry->inCombinedRenderableProviderBlocks[providerType];

			if (inCombinedRenderableBlock != nullptr && inCombinedRenderableBlock->memBlock != nullptr) {
				inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(providerType, gbEntry, nullptr/*gpInstance_forModelTransforms*/);
			}
		}

		gbEntry->geometryBucket = nullptr;
		// Do a delete on the Entry_GeometryManagerBucket here because the grouping information is no longer required.
		// The object gets deleted during the remove() call, don't acess it after.
		gbEntry->inGeometryBucket_listEntry->remove();
		//entry_geometryBucket->inGeometryBucket_listEntry = nullptr;
	}

	// Remove model_transforms using toRemove_GPInstancesList.
	for (int providerType = 0; providerType < GEOMETRY_PROVIDER_TYPES_COUNT; providerType++) {
		ListDL<GeometryProviderInstance*>& toRemoveEntriesList = toRemove_GPInstancesList[providerType];

		while (toRemoveEntriesList.count() > 0) {
			ListDL<GeometryProviderInstance*>::Entry* entry = toRemoveEntriesList.getFirst();
			GeometryProviderInstance* gpInstance = entry->data;

			if (gpInstance->geometryBucket != this || gpInstance->inGeometryBucket_listEntry == nullptr) {
				throw LogicException(LOC);
			}

			if (gpInstance->inGMan_invalidation_listEntry != nullptr) {
				// Cancel invalidation, no longer required for this gpi.
				gpInstance->invalidationBitMask = 0;

				gpInstance->inGMan_invalidation_listEntry->remove();
				gpInstance->inGMan_invalidation_listEntry = nullptr;
			}

			if (gpInstance->providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
				// Only the model_transforms are removed from cRenderable with this method. The other types are removed with the method above because they are .
				for (int cRendUIndex = 0; cRendUIndex < gpInstance->inCombinedRenderableBlockIndexedArray->count(); cRendUIndex++) {
					InCombinedRenderableBlock* inCombinedRenderableBlock = gpInstance->inCombinedRenderableBlockIndexedArray->getDirect(cRendUIndex);

					if (inCombinedRenderableBlock != nullptr) {
						ICRB_ModelTransforms* icrb_ModelTransforms = static_cast<ICRB_ModelTransforms*>(inCombinedRenderableBlock->rawSubclassPointer);

						if (icrb_ModelTransforms->memBlock_transformsIndexRemaping != nullptr) {
							// Release the remaping slot from the cRenderable.
							icrb_ModelTransforms->memBlock_transformsIndexRemaping->memMan->combinedRenderable->releaseTransformsRemapingSlot(icrb_ModelTransforms);
						}

						if (inCombinedRenderableBlock->memBlock != nullptr) {
							inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(gpInstance->providerType, nullptr/*entryGBucket_forVerticesIndices*/, gpInstance);
						}
					}
				}
			}

			gpInstance->geometryBucket = nullptr;
			// The remove() call will delete the entry, don't acess it after.
			gpInstance->inGeometryBucket_listEntry->remove();
			// gpInstance->inGeometryBucket_listEntry = nullptr;

			entry->remove();
		}
	}
}

void GB_Validation::tickLogic_invalidationGProviderInstances() {
	//Enumerate all invalid gpInstances and remove them from all cRenderables.
	for (int providerType = 0; providerType < GEOMETRY_PROVIDER_TYPES_COUNT; providerType++) {
		ListDL<GeometryProviderInstance*>& invalidEntriesList = invalidGPInstancesListsArray.getReference(providerType);

		// Enumerate all invalid entries for each provider type.
		for (ListDL<GeometryProviderInstance*>::Entry* entry = invalidEntriesList.getFirst(); entry != nullptr; entry = entry->next) {
			GeometryProviderInstance* gpInstance = entry->data;

			if (providerType == IGeometryProvider_Vertices::PROVIDER_TYPE) {
				// Enumerate all bucket entries that use the gpInstance.
				for (ListDL<Entry_GeometryManagerBucket*>::Entry* entry = gpInstance->connectedGMGroupsList.getFirst(); entry != nullptr; entry = entry->next) {
					Entry_GeometryManagerBucket* entryBucket = entry->data;

					InCombinedRenderableBlock* inCombinedRenderableBlock = entryBucket->inCombinedRenderableProviderBlocks[providerType];

					if (inCombinedRenderableBlock != nullptr && inCombinedRenderableBlock->memBlock != nullptr) {
						inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(gpInstance->providerType, entryBucket, gpInstance);
					}
				}
			} else if (providerType == IGeometryProvider_Indices::PROVIDER_TYPE) {
				if ((gpInstance->invalidationBitMask & IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup) == IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup) {
					// Only remove indices memBlock on count invalidation.
					// Enumerate all bucket entries that use the gpInstance.
					for (ListDL<Entry_GeometryManagerBucket*>::Entry* entry = gpInstance->connectedGMGroupsList.getFirst(); entry != nullptr; entry = entry->next) {
						Entry_GeometryManagerBucket* entryBucket = entry->data;

						InCombinedRenderableBlock* inCombinedRenderableBlock = entryBucket->inCombinedRenderableProviderBlocks[providerType];

						if (inCombinedRenderableBlock != nullptr && inCombinedRenderableBlock->memBlock != nullptr) {
							inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(gpInstance->providerType, entryBucket, gpInstance);
						}
					}
				}
			} else if (providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
				for (int i = 0; i < gpInstance->inCombinedRenderableBlockIndexedArray->count(); i++) {
					InCombinedRenderableBlock* inCombinedRenderableBlock = gpInstance->inCombinedRenderableBlockIndexedArray->getDirect(i, nullptr);

					//asd_01;// Use multiple types of removal. Need to keep transforms remaping index slot if transform_dataSameCount invalidation occured.
					//asd_01;/// Need to release the transforms remaping slot after antries processing finishes if transform will notr be in the cRenderable after.
					//asd_01;/// Use a partial removal that releases only the data blocks and keeps the remaping slot.
					if (inCombinedRenderableBlock != nullptr) {
						ICRB_ModelTransforms* icrb_ModelTransforms = static_cast<ICRB_ModelTransforms*>(inCombinedRenderableBlock->rawSubclassPointer);

						if (icrb_ModelTransforms->memBlock_transformsIndexRemaping != nullptr && (gpInstance->invalidationBitMask & IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup) == IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup) {
							// Release the remaping slot from the cRenderable when a invalidation_countAndData_bitGroup occurs.
							icrb_ModelTransforms->memBlock_transformsIndexRemaping->memMan->combinedRenderable->releaseTransformsRemapingSlot(icrb_ModelTransforms);
						}

						if (inCombinedRenderableBlock->memBlock != nullptr) {
							inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(gpInstance->providerType, nullptr/*entryGBucket_forVerticesIndices*/, gpInstance);
						}
					}
				}
			} else {
				throw LogicException(LOC);
			}

			// Make this 0 after the updates because it is required in the update logic (eg. to update the vertices.a_modelTransIndex).
			//gpInstance->invalidationBitMask = 0;
		}
	}
}

void GB_Validation::tickLogic_prepareCRenderablesForEntriesEnumerationInsertionPhase() {
	for (ListDL<CombinedRenderable>::Entry* entry = combinedRenderablesList.getFirst(); entry != nullptr; entry = entry->next) {
		// This is used for a optimisation to avoid indices re-writing if not needed.
		/// The logic will detect if indices are already where they should be.
		entry->data.prepareForEnumerationInsertionPhase();
	}
}

void GB_Validation::tickLogic_combineEntriesNoDataWrites() {
	// Enumerate all renderables.
	//asd_01;// the current cRenderables are closely linked to the entries ones.
	CombinedRenderable* combinedRenderable = nullptr;
	// Always grab the next cRend to fast_check if the entry is in the next cRend already.
	CombinedRenderable* nextCombinedRenderable = nullptr;
	int entryIndex = -1;// This is for debug.
	for (ListDL<Entry_GeometryManagerBucket>::Entry* entry = entriesList_connectedGroups.getFirst(); entry != nullptr; entry = entry->next) {
		Entry_GeometryManagerBucket* entryGBucket = &entry->data;
		entryIndex++;

		GeometryProviderInstance* gpi_Vertices = entryGBucket->gpInstancesArray[IGeometryProvider_Vertices::PROVIDER_TYPE].gpInstances;
		GeometryProviderInstance* gpi_Indices = entryGBucket->gpInstancesArray[IGeometryProvider_Indices::PROVIDER_TYPE].gpInstances;
		GeometryProviderInstance* gpi_ModelTransforms = entryGBucket->gpInstancesArray[IGeometryProvider_ModelTransforms::PROVIDER_TYPE].gpInstances;

		// Even if the current cRenderable has capacity, jump to the next one if the entry is already in that one and the current cRend fill_rate is > ~85%.
		// Use a while to handle the case where the next one or more cRenderable doesn't have enough space and get one that is further away.
		while (
			combinedRenderable == nullptr /*no_cRend_exists*/
			|| !combinedRenderable->hasCapacityForGeometryBucketEntry(entryGBucket, gpi_Vertices, gpi_Indices, gpi_ModelTransforms)/*cRend_is_full*/
			|| (entryGBucket->usesCombinedRenderable != nullptr && entryGBucket->usesCombinedRenderable == nextCombinedRenderable/*entry_is_already_in_next_cRend*/ && combinedRenderable->getFillRatePercent() >= NextCRendJumpIfAlreadyInItFillRate))/*current_cRend_is_full_enough*/
		{
			combinedRenderable = getNextCombinedRenderable(
				combinedRenderable,
				gpi_Vertices, gpi_Indices, gpi_ModelTransforms
			);
			nextCombinedRenderable = combinedRenderable->inGeometryManager_listEntry->next != nullptr ? &combinedRenderable->inGeometryManager_listEntry->next->data : nullptr;
		}
		
		if (entryGBucket->usesCombinedRenderable != nullptr && entryGBucket->usesCombinedRenderable != combinedRenderable) {
			entryGBucket->usesCombinedRenderable->removeSemiManagedGeometryBucketEntry(
				entryGBucket, 
				gpi_Vertices, gpi_Indices, gpi_ModelTransforms, 
				markedForRemovalGPIModelTransformsList
			);
		}

		// Append each renderable to a combined_renderable.
		combinedRenderable->smartPushGeometryBucketEntry(entryGBucket, gpi_Vertices, gpi_Indices, gpi_ModelTransforms);
	}
}

void GB_Validation::tickLogic_defragmentations() {
	for (ListDL<CombinedRenderable>::Entry* entry = combinedRenderablesList.getFirst(); entry != nullptr; entry = entry->next) {
		CombinedRenderable* cRend = &entry->data;

		//asd_01;// add some params for entries count and bandwidth, maybe use a class that contains current state.
		cRend->tickDefrag();
	}
}

void GB_Validation::tickLogic_writeChangedDatas() {
	for (ListDL<CombinedRenderable>::Entry* entry = combinedRenderablesList.getFirst(); entry != nullptr; entry = entry->next) {
		CombinedRenderable* cRend = &entry->data;

		cRend->writeChangedDatas();
	}
}

void GB_Validation::tickLogic_clearGPInstancesInvalidationFlags() {
	for (int providerType = 0; providerType < invalidGPInstancesListsArray.count(); providerType++) {
		ListDL<GeometryProviderInstance*>& invalidEntriesList = invalidGPInstancesListsArray.getReference(providerType);

		while (invalidEntriesList.count() > 0) {
			ListDL<GeometryProviderInstance*>::Entry* entry = invalidEntriesList.getFirst();
			GeometryProviderInstance* gpInstance = entry->data;

			gpInstance->invalidationBitMask = 0;

			gpInstance->inGMan_invalidation_listEntry->remove();
			gpInstance->inGMan_invalidation_listEntry = nullptr;
		}
	}
}

void GB_Validation::tickLogic_handleDelayedICRModelTransformsRemoval() {
	// Enumerate the marked for removal gpi_mTrans entries and remove them (they are guaranteed to not be used).
	while(markedForRemovalGPIModelTransformsList.count() > 0) {
		ListDL<ICRB_ModelTransforms::ToRemoveUnusedEntry>::Entry* entry = markedForRemovalGPIModelTransformsList.getFirst();
		ICRB_ModelTransforms::ToRemoveUnusedEntry* toRemoveEntry = &entry->data;

		if (toRemoveEntry->gpi_ModelTransforms->geometryBucket != this || toRemoveEntry->gpi_ModelTransforms->inGeometryBucket_listEntry == nullptr) {
			throw LogicException(LOC);
		}

		if (toRemoveEntry->gpi_ModelTransforms->inGMan_invalidation_listEntry != nullptr) {
			// Not sure if this can occur without a bug. Check when this occurs.
			throw LogicException(LOC);
		}

		// Only the model_transforms are removed from cRenderable with this method. The other types are removed with the method above because they are customised single_use and this can be used in multiple places.
		ICRB_ModelTransforms* inCombinedRenderableBlock = toRemoveEntry->icrb_modelTransforms;
		if (inCombinedRenderableBlock != nullptr) {
			if (inCombinedRenderableBlock->inGBucket_toRemoveUnused_listEntry == nullptr) {
				throw LogicException(LOC);
			}

			ICRB_ModelTransforms* icrb_ModelTransforms = static_cast<ICRB_ModelTransforms*>(inCombinedRenderableBlock->rawSubclassPointer);
			icrb_ModelTransforms->inGBucket_toRemoveUnused_listEntry = nullptr;

			if (icrb_ModelTransforms->memBlock_transformsIndexRemaping != nullptr) {
				// Release the remaping slot from the cRenderable.
				icrb_ModelTransforms->memBlock_transformsIndexRemaping->memMan->combinedRenderable->releaseTransformsRemapingSlot(icrb_ModelTransforms);
			}

			if (inCombinedRenderableBlock->memBlock != nullptr) {
				inCombinedRenderableBlock->memBlock->memMan->combinedRenderable->removeGeometryProviderInstanceDataMustExist(toRemoveEntry->gpi_ModelTransforms->providerType, nullptr/*entryGBucket_forVerticesIndices*/, toRemoveEntry->gpi_ModelTransforms);
			}

			// This is the location where gpInstance->inCombinedRenderableBlockIndexedArray entries can be deleted.
			delete inCombinedRenderableBlock;

			toRemoveEntry->gpi_ModelTransforms->inCombinedRenderableBlockIndexedArray->setDirect(toRemoveEntry->cRendUIndex, nullptr);
		}

		bool isAnyIcrbUsed = false;
		for (int cRendUIndex = 0; cRendUIndex < toRemoveEntry->gpi_ModelTransforms->inCombinedRenderableBlockIndexedArray->count(); cRendUIndex++) {
			InCombinedRenderableBlock* icrb = toRemoveEntry->gpi_ModelTransforms->inCombinedRenderableBlockIndexedArray->getDirect(cRendUIndex);
			if(icrb != nullptr) {
				if (icrb->unusedDetectionUseCount == 0) {
					// Some logic error occured probably.
					throw LogicException(LOC);
				}

				isAnyIcrbUsed = true;
				break;
			}
		}

		if (!isAnyIcrbUsed) {
			toRemoveEntry->gpi_ModelTransforms->geometryBucket = nullptr;
			// The remove() call will delete the entry, don't acess it after.
			toRemoveEntry->gpi_ModelTransforms->inGeometryBucket_listEntry->remove();
			// gpInstance->inGeometryBucket_listEntry = nullptr;
		}

		entry->remove();
	}
}

void GB_Validation::prepareForRemoval() {
	tickLogic_toRemoveEntries();
}

GB_Validation::~GB_Validation() {
	//void
}
