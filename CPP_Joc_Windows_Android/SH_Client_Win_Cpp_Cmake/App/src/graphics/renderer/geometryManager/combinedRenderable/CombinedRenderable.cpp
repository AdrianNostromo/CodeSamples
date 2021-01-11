#include "CombinedRenderable.h"
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/renderer/renderPhase/util/IRenderable.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/geometry/vertexAttribute/VertexAttribute.h>
#include <graphics/model/INodePart.h>
#include <graphics/renderer/geometryManager/geometryProviders/instance/GeometryProviderInstance.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Vertices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_Indices.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider_ModelTransforms.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <graphics/geometry/data/UniformBufferObject.h>
#include <memory>
#include <base/list/CompositesBuffer.h>
#include <cassert>
#include <base/log/GlobalAccessLogHandler.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/InCombinedRenderableBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Vertices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_Indices.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>
#include <graphics/renderer/geometryManager/util/Entry_GeometryManagerBucket.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/GeometryBufferManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AllocatedMemBlock.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/BlockMemoryManager.h>
#include <graphics/renderer/geometryManager/combinedRenderable/memMan/blockMemoryManager/util/AnyMemBlock.h>
#include <base/math/Bounds1DInt.h>

using namespace graphics;

ListDL<CombinedRenderable*> CombinedRenderable::globalInstancesList{};

CombinedRenderable::CombinedRenderable(
	std::shared_ptr<VertexAttributesList> vertexAttributesList,
	std::shared_ptr<graphics::Material> material,
	int maxVerticesCount, int maxIndicesCount, int maxWorldTransformsCount)
	: super(vertexAttributesList,
		material,
		maxVerticesCount, maxIndicesCount, maxWorldTransformsCount)
{
	//void
}

void CombinedRenderable::create() {
	super::create();

	if (mm_modelTransforms == nullptr) {
		// This is a 2d cRenderable, find insertion location.

		ListDL<CombinedRenderable*>::Entry* insertAfterEntry = nullptr;
		for (ListDL<CombinedRenderable*>::Entry* entry = globalInstancesList.getFirst(); entry != nullptr; entry = entry->next) {
			if (entry->data->mm_modelTransforms == nullptr) {
				// A 2d entry encountered, save it as the insert after entry.
				insertAfterEntry = entry;
			} else {
				// A 3d entry encountered, insert before it.
				break;
			}
		}
		if (insertAfterEntry != nullptr) {
			inGlobalList_cEntry = globalInstancesList.insertAfterDirect(insertAfterEntry, this);
		} else {
			inGlobalList_cEntry = globalInstancesList.prependDirect(this);
		}
	} else {
		// This is a 3d cRend, append it at the end.
		inGlobalList_cEntry = globalInstancesList.appendDirect(this);
	}
}

void CombinedRenderable::prepareForEnumerationInsertionPhase() {
	lastEnumerationProcessedMemBlock_indices = nullptr;
	lastEnumerationProcessedMemBlock_modelTransforms = nullptr;
}

bool CombinedRenderable::hasCapacityForGeometryBucketEntry(
	Entry_GeometryManagerBucket* entryGBucket,
	GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms)
{
	if (entryGBucket->getInCombinedRenderableBlockIfUsed(IGeometryProvider_Vertices::PROVIDER_TYPE, uIndex) == nullptr/*not_in_combined_renderable*/ && static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVerticesCount() > mm_vertices->getInsertEntrisCapacityWithAllocationInsertionType()/*insuficient_space*/) {
		return false;
	}
	
	// Also include the fragment entries count because the fragment are re-used entirely. That is automatically done by getInsertEntrisCapacityWithAllocationInsertionType().
	if (entryGBucket->getInCombinedRenderableBlockIfUsed(IGeometryProvider_Indices::PROVIDER_TYPE, uIndex) == nullptr/*not_in_combined_renderable*/ 
		&& static_cast<IGeometryProvider_Indices*>(gpi_Indices->customProviderRawPtr)->getIndicesCount() > mm_indices->getInsertEntrisCapacityWithAllocationInsertionType()/*insuficient_space*/)
	{
		return false;
	}
	
	if (mm_modelTransforms != nullptr) {
		if (static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getWorldTransformOptional() != nullptr) {
			if (static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getModelTransformsUBO() != nullptr || static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getModelNormalTransformsUBO() != nullptr) {
				// Only one transforms type can be used at a time.
				throw LogicException(LOC);
			}

			if (entryGBucket->getInCombinedRenderableBlockIfUsed(IGeometryProvider_ModelTransforms::PROVIDER_TYPE, uIndex) == nullptr/*not_in_combined_renderable*/ && 1 > mm_modelTransforms->getInsertEntrisCapacityWithAllocationInsertionType()/*insuficient_space*/) {
				return false;
			}
		} else {
			// Combining entries can have only 1 transform. The transforms array is used only in combined_renderables.
			throw LogicException(LOC);
		}
	} else {
		if (gpi_ModelTransforms != nullptr && static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getWorldTransformOptional() != nullptr) {
			throw LogicException(LOC);
		}
	}

	// Note. No need to check model normal transforms separately because it has a mirrored structure to modelTransforms.

	return true;
}

void CombinedRenderable::smartPushGeometryBucketEntry(
	Entry_GeometryManagerBucket* entryGBucket,
	GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms)
{
	// Check that the renderable has the exact same vAttributes.
	// This direct check works because the vAttributes are sorted.
	if (mm_vertices->getBufferObject_asVBO()->vertexAttributesList->getTypesBitMask() != static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVertexAttributesList()->getTypesBitMask()) {
		// VAttributes list must be identical.
		throw LogicException(LOC);
	}

	if (entryGBucket->usesCombinedRenderable != nullptr && entryGBucket->usesCombinedRenderable != this) {
		// The entry is in another cRenderable, remove it from that one first.
		throw LogicException(LOC);
	}

	bool isDataDirty_vertices = false;
	bool isDataDirty_indices = false;

	InCombinedRenderableBlock* icrb_vertices = entryGBucket->getOrCreateInCombinedRenderableBlock(IGeometryProvider_Vertices::PROVIDER_TYPE, uIndex);
	{
		if (entryGBucket->usesCombinedRenderable == nullptr) {
			// The use detection system is designed for mTrans delayed removal, use it here as a cheap error detection.
			// For verts, indices, the use count is incremented on write because 
			if (icrb_vertices->unusedDetectionUseCount != 0) {
				// The icrb is already written somehwere. Logic error.
				throw LogicException(LOC);
			}

			icrb_vertices->unusedDetectionUseCount++;
		}

		int insertVerticesCount = -1;
		if (icrb_vertices->memBlock == nullptr && (insertVerticesCount = static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVerticesCount()) >= 1) {
			// Insert.
			if (insertVerticesCount > mm_vertices->getInsertEntrisCapacityWithAllocationInsertionType()) {
				throw LogicException(LOC);
			}

			icrb_vertices->memBlock = mm_vertices->allocate(insertVerticesCount, entryGBucket/*dataSource_bucketEntry*/, nullptr/*dataSource_gpi_forMt*/, nullptr/*dataSource_icrb_forRemapingMt*/);
			// If he vertices offset changed, the indices require to be updated.
			isDataDirty_indices = true;

			isDataDirty_vertices = true;

			icrb_vertices->memBlock->invalidateData(AllocatedMemBlock::DirtyDataBitGroups::Main);
		}
	}

	InCombinedRenderableBlock* icrb_indices = entryGBucket->getOrCreateInCombinedRenderableBlock(IGeometryProvider_Indices::PROVIDER_TYPE, uIndex);
	{
		if (entryGBucket->usesCombinedRenderable == nullptr) {
			// The use detection system is designed for mTrans delayed removal, use it here as a cheap error detection.
			// For verts, indices, the use count is incremented on write because 
			if (icrb_indices->unusedDetectionUseCount != 0) {
				// The icrb is already written somehwere. Logic error.
				throw LogicException(LOC);
			}

			icrb_indices->unusedDetectionUseCount++;
		}

		// Note. If the memBlock_indices exists, it is guaranteed that the indices count didn't change (because the memBlock is removed on that invalidation).
		/// For this reason, if the memBLock_indices exists and is at the correct location, it can be assumed it is valid and doesn't need to be deallocated and allocated by the following code logic.
		int indicesInsertCount = -1;
		if ((indicesInsertCount = static_cast<IGeometryProvider_Indices*>(gpi_Indices->customProviderRawPtr)->getIndicesCount()) >= 1) {
			// A region will be required. Check if it is clear or if the pre-existing allocation is at the correct location.
			if (icrb_indices->memBlock == nullptr 
				|| (lastEnumerationProcessedMemBlock_indices == nullptr && icrb_indices->memBlock->blockEntriesOffsetSideAligned != 0)
				|| (lastEnumerationProcessedMemBlock_indices != nullptr && icrb_indices->memBlock->blockEntriesOffsetSideAligned != lastEnumerationProcessedMemBlock_indices->blockEntriesOffsetSideAligned + lastEnumerationProcessedMemBlock_indices->blockEntriesCount))
			{
				// The indices are not where they should be, re-allocation required.
				handleDesiredIndicesRegionClearing(icrb_indices->memBlock, indicesInsertCount);
			}
		}
		
		if (icrb_indices->memBlock == nullptr 
			// This is to initialise the variable value.
			&& (indicesInsertCount != -1 || (indicesInsertCount = static_cast<IGeometryProvider_Indices*>(gpi_Indices->customProviderRawPtr)->getIndicesCount()) != -1)
			&& indicesInsertCount >= 1)
		{
			if (indicesInsertCount > mm_indices->getInsertEntrisCapacityWithAllocationInsertionType()) {
				throw LogicException(LOC);
			}

			icrb_indices->memBlock = mm_indices->allocate(indicesInsertCount, entryGBucket/*dataSource_bucketEntry*/, nullptr/*dataSource_gpi_forMt*/, nullptr/*dataSource_icrb_forRemapingMt*/);
			if ((lastEnumerationProcessedMemBlock_indices == nullptr && icrb_indices->memBlock->blockEntriesOffsetSideAligned != 0)
				|| (lastEnumerationProcessedMemBlock_indices != nullptr && icrb_indices->memBlock->blockEntriesOffsetSideAligned != lastEnumerationProcessedMemBlock_indices->blockEntriesOffsetSideAligned + lastEnumerationProcessedMemBlock_indices->blockEntriesCount))
			{
				// The allocated region must be in order and after lastEnumerationProcessedMemBlock_indices.
				throw LogicException(LOC);
			}

			isDataDirty_indices = true;
		}

		if (icrb_indices->memBlock != nullptr) {
			// Ignore the indices entries that dhave a indices count of 0.
			lastEnumerationProcessedMemBlock_indices = icrb_indices->memBlock;

			if (isDataDirty_indices) {
				icrb_indices->memBlock->invalidateData(AllocatedMemBlock::DirtyDataBitGroups::Main);
			}
		}
	}

	if (mm_modelTransforms != nullptr) {
		ICRB_ModelTransforms* icrb_modelTransforms = static_cast<ICRB_ModelTransforms*>(entryGBucket->getOrCreateInCombinedRenderableBlock(IGeometryProvider_ModelTransforms::PROVIDER_TYPE, uIndex)->rawSubclassPointer);
		bool isDataDirty_mTrans = false;
		bool isNewDataAllocation_mTrans = false;
		bool isDataDirty_remapingSlotMTrans = false;
		bool isNewDataAllocation_remapingSlotMTrans = false;

		if (entryGBucket->usesCombinedRenderable == nullptr) {
			// Entry_cRend for mTransf uses a delayed removal, so if this entrywas marked for removal (because not used), increment it's use count and remove it from marked for removals list.
			icrb_modelTransforms->unusedDetectionUseCount++;
			if (icrb_modelTransforms->unusedDetectionUseCount == 1 && icrb_modelTransforms->inGBucket_toRemoveUnused_listEntry != nullptr) {
				icrb_modelTransforms->inGBucket_toRemoveUnused_listEntry->remove();
				icrb_modelTransforms->inGBucket_toRemoveUnused_listEntry = nullptr;
			}
		}

		int modelTransformsToInsertCount = 1;
		if (icrb_modelTransforms->memBlock == nullptr && modelTransformsToInsertCount >= 1) {
			if (modelTransformsToInsertCount > mm_modelTransforms->getInsertEntrisCapacityWithAllocationInsertionType()) {
				throw LogicException(LOC);
			}

			icrb_modelTransforms->memBlock = mm_modelTransforms->allocate(modelTransformsToInsertCount, nullptr/*dataSource_bucketEntry*/, gpi_ModelTransforms/*dataSource_gpi_forMt*/, nullptr/*dataSource_icrb_forRemapingMt*/);
			isNewDataAllocation_mTrans = true;

			isDataDirty_mTrans = true;

			icrb_modelTransforms->memBlock->invalidateData(AllocatedMemBlock::DirtyDataBitGroups::Main);

			if (mm_modelTransforms->bufferObjectB != nullptr) {
				// Update the optional normalModelTrans_buffer.
				// Normal model trans is ustomatically update when the mTrans is updated.
			}
		}
		if (icrb_modelTransforms->memBlock != nullptr) {
			lastEnumerationProcessedMemBlock_modelTransforms = icrb_modelTransforms->memBlock;
		}

		if (mm_remapingModelTransformsIndices != nullptr) {
			if (icrb_modelTransforms->memBlock_transformsIndexRemaping == nullptr && modelTransformsToInsertCount >= 1) {
				// Reserve a remaping transforms index entry and write that slot to the vertices.

				icrb_modelTransforms->memBlock_transformsIndexRemaping = mm_remapingModelTransformsIndices->allocate(1/*blockEntriesCount*/, nullptr/*dataSource_bucketEntry*/, nullptr/*dataSource_gpi_forMt*/, icrb_modelTransforms/*dataSource_icrb_forRemapingMt*/);
				isNewDataAllocation_remapingSlotMTrans = true;

				isDataDirty_remapingSlotMTrans = true;
			}

			if (isDataDirty_remapingSlotMTrans || isNewDataAllocation_mTrans) {
				// This is the index of the component dirctly as a global index in the array without the vec4 subtype.

				icrb_modelTransforms->memBlock_transformsIndexRemaping->invalidateData(AllocatedMemBlock::DirtyDataBitGroups::Main);
			}

			if (isDataDirty_vertices || isNewDataAllocation_remapingSlotMTrans) {
				// Update the vertices to have the required slot.

				icrb_vertices->memBlock->invalidateData(AllocatedMemBlock::DirtyDataBitGroups::RemapingMTransSlot);
			}
		}
	}

	// Save the val here so the previous ogic can detect when the first use start occurs.
	entryGBucket->usesCombinedRenderable = this;
}

void CombinedRenderable::handleDesiredIndicesRegionClearing(AllocatedMemBlock* existingtMemBlock, int entriesCount) {
	if (lastEnumerationProcessedMemBlock_indices == nullptr) {
		// Inserting the first entry.

		if (existingtMemBlock != nullptr && existingtMemBlock->blockEntriesOffsetSideAligned == 0) {
			// Already at desired location.
			return;
		}
		
		// Not at the desired location. Check that the target region is clear.
		if (existingtMemBlock != nullptr) {
			// Remove the existing mem block entry.

			removeGeometryProviderInstanceDataMustExist(
				IGeometryProvider_Indices::PROVIDER_TYPE,
				existingtMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
			);
		}

		// Clear entries from the start of the active_allocations_tree until enough entries exist.
		// If the first entry exists in allMemBlocksList, it must be a empty fragment large enough for the insert.
		ListDL<AnyMemBlock>::Entry* firstMemBlock = nullptr;
		while ((firstMemBlock = mm_indices->blockMM_volatile->allMemBlocksList.getFirst()) != nullptr/*first_entry_exists*/) {
			if (firstMemBlock->data.allocatedMemBlock != nullptr) {
				// The first entry is not empty, release it.
				removeGeometryProviderInstanceDataMustExist(
					IGeometryProvider_Indices::PROVIDER_TYPE,
					firstMemBlock->data.allocatedMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
				);
			} else if(firstMemBlock->data.blockEntriesCount < entriesCount/*is_too_small*/) {
				// Grab the second entry and release it.
				ListDL<AnyMemBlock>::Entry* secondEmptyMemBlock = firstMemBlock->next;
				if (secondEmptyMemBlock == nullptr || secondEmptyMemBlock->data.allocatedMemBlock == nullptr) {
					// The second entry must exist and be a allocated block at this location.
					throw LogicException(LOC);
				}

				removeGeometryProviderInstanceDataMustExist(
					IGeometryProvider_Indices::PROVIDER_TYPE,
					secondEmptyMemBlock->data.allocatedMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
				);
			} else {
				// First entry is large enough.
				break;
			}
		}
	} else {
		if (existingtMemBlock != nullptr 
			&& existingtMemBlock->blockEntriesOffsetSideAligned == lastEnumerationProcessedMemBlock_indices->blockEntriesOffsetSideAligned + lastEnumerationProcessedMemBlock_indices->blockEntriesCount)
		{
			// Already at desired location after lastEnumerationProcessedMemBlock_indices.
			return;
		}

		// Not at the desired location. Check that the target region is clear.
		if (existingtMemBlock != nullptr) {
			// Remove the existing mem block.

			removeGeometryProviderInstanceDataMustExist(
				IGeometryProvider_Indices::PROVIDER_TYPE,
				existingtMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
			);
		}

		// Clear entries after lastEnumerationProcessedMemBlock_indices from the active_allocations_tree until enough entries exist.
		ListDL<AnyMemBlock>::Entry* nextMemBlock = nullptr;
		while ((nextMemBlock = lastEnumerationProcessedMemBlock_indices->memBlockAny->selfCEntry->next) != nullptr/*first_entry_exists*/) {

			if (nextMemBlock->data.allocatedMemBlock != nullptr) {
				// The first entry is not empty, release it.
				removeGeometryProviderInstanceDataMustExist(
					IGeometryProvider_Indices::PROVIDER_TYPE,
					nextMemBlock->data.allocatedMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
				);
			} else if (nextMemBlock->data.blockEntriesCount < entriesCount/*is_too_small*/) {
				// Grab the second entry and release it.
				ListDL<AnyMemBlock>::Entry* secondEmptyMemBlock = nextMemBlock->next;
				if (secondEmptyMemBlock == nullptr || secondEmptyMemBlock->data.allocatedMemBlock == nullptr) {
					// The second entry must exist and be a allocated block at this location.
					throw LogicException(LOC);
				}

				removeGeometryProviderInstanceDataMustExist(
					IGeometryProvider_Indices::PROVIDER_TYPE,
					secondEmptyMemBlock->data.allocatedMemBlock->dataSource_bucketEntry_forVI/*entryGBucket_forVerticesIndices*/, nullptr/*gpInstance_forModelTransforms*/
				);
			} else {
				// First entry is large enough.
				break;
			}
		}
	}
}

void CombinedRenderable::removeSemiManagedGeometryBucketEntry(
	Entry_GeometryManagerBucket* entryGBucket,
	GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices, GeometryProviderInstance* gpi_ModelTransforms,
	ListDL<ICRB_ModelTransforms::ToRemoveUnusedEntry>& markedForRemovalGPIModelTransformsList)
{
	if (entryGBucket == nullptr || entryGBucket->usesCombinedRenderable != this) {
		throw LogicException(LOC);
	}
	entryGBucket->usesCombinedRenderable = nullptr;

	// This does instant removal for vertices, indices and does delayed list removal for mTransforms.

	{
		InCombinedRenderableBlock* inCombinedRenderableBlock = entryGBucket->getInCombinedRenderableBlockIfExists(IGeometryProvider_Vertices::PROVIDER_TYPE, uIndex);
		if (inCombinedRenderableBlock == nullptr || inCombinedRenderableBlock->unusedDetectionUseCount != 1) {
			// Because entryGBucket->usesCombinedRenderable exists, it is guaranteed that vertices count should be 1 at this location.
			throw LogicException(LOC);
		}
		inCombinedRenderableBlock->unusedDetectionUseCount--;

		if (inCombinedRenderableBlock->memBlock != nullptr) {
			removeGeometryProviderInstanceDataMustExist(IGeometryProvider_Vertices::PROVIDER_TYPE, entryGBucket, gpi_Vertices);
		}
	}

	{
		InCombinedRenderableBlock* inCombinedRenderableBlock = entryGBucket->getInCombinedRenderableBlockIfExists(IGeometryProvider_Indices::PROVIDER_TYPE, uIndex);
		if (inCombinedRenderableBlock == nullptr || inCombinedRenderableBlock->unusedDetectionUseCount != 1) {
			// Because entryGBucket->usesCombinedRenderable exists, it is guaranteed that indices count should be 1 at this location.
			throw LogicException(LOC);
		}
		inCombinedRenderableBlock->unusedDetectionUseCount--;

		if (inCombinedRenderableBlock->memBlock != nullptr) {
			removeGeometryProviderInstanceDataMustExist(IGeometryProvider_Indices::PROVIDER_TYPE, entryGBucket, gpi_Indices);
		}
	}

	if (mm_modelTransforms != nullptr) {
		InCombinedRenderableBlock* inCombinedRenderableBlock = entryGBucket->getInCombinedRenderableBlockIfExists(IGeometryProvider_ModelTransforms::PROVIDER_TYPE, uIndex);
		if (inCombinedRenderableBlock == nullptr || inCombinedRenderableBlock->unusedDetectionUseCount <= 0) {
			// Because entryGBucket->usesCombinedRenderable exists, it is guaranteed that the count should be greater than 1.
			throw LogicException(LOC);
		}

		ICRB_ModelTransforms* icrb_modelTransforms = static_cast<ICRB_ModelTransforms*>(inCombinedRenderableBlock);
		
		icrb_modelTransforms->unusedDetectionUseCount--;

		if (icrb_modelTransforms->unusedDetectionUseCount == 0) {
			// Mark modelTrans for removal.
			icrb_modelTransforms->inGBucket_toRemoveUnused_listEntry = markedForRemovalGPIModelTransformsList.appendEmplace(gpi_ModelTransforms, icrb_modelTransforms, uIndex);
		}
	}
}

void CombinedRenderable::removeGeometryProviderInstanceDataMustExist(
	int providerType, 
	Entry_GeometryManagerBucket* entryGBucket_forVerticesIndices, GeometryProviderInstance* gpInstance_forModelTransforms)
{
	InCombinedRenderableBlock* inCombinedRenderableBlock = nullptr;
	if ((providerType == IGeometryProvider_Vertices::PROVIDER_TYPE || providerType == IGeometryProvider_Indices::PROVIDER_TYPE) && entryGBucket_forVerticesIndices != nullptr) {
		inCombinedRenderableBlock = entryGBucket_forVerticesIndices->getInCombinedRenderableBlockIfUsed(providerType, uIndex);
	} else if(providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE && gpInstance_forModelTransforms != nullptr) {
		// For model_trans, the inCombinedRenderableBlock can be grabed from gpInstance without the need to use the optional entryGBucket.
		/// Requiring entryGBucket whould require to fetch any Entry_GeometryManagerBucket that uses gpInstance and then grab inCombinedRenderableBlock from the gpInstance that is in the entryGBucket and that is inneficient and seems wrong.
		inCombinedRenderableBlock = gpInstance_forModelTransforms->inCombinedRenderableBlockIndexedArray->getDirect(uIndex, nullptr);
	} else {
		throw LogicException(LOC);
	}
	if (inCombinedRenderableBlock == nullptr || inCombinedRenderableBlock->memBlock == nullptr || inCombinedRenderableBlock->memBlock->memMan->combinedRenderable != this) {
		throw LogicException(LOC);
	}

	if (providerType == IGeometryProvider_Vertices::PROVIDER_TYPE) {
		// Remove the vertices entry from the cRend.
		ICRB_Vertices* icrb_Vertices = static_cast<ICRB_Vertices*>(inCombinedRenderableBlock->rawSubclassPointer);

		inCombinedRenderableBlock->memBlock = mm_vertices->deallocate(icrb_Vertices->memBlock);
	} else if (providerType == IGeometryProvider_Indices::PROVIDER_TYPE) {
		ICRB_Indices* icrb_Indices = static_cast<ICRB_Indices*>(inCombinedRenderableBlock->rawSubclassPointer);

		// Indices use a comblete buffer clear and re-creation, there is no mem manager structures for indices currently.
		inCombinedRenderableBlock->memBlock = mm_indices->deallocate(icrb_Indices->memBlock);
	} else if (providerType == IGeometryProvider_ModelTransforms::PROVIDER_TYPE) {
		ICRB_ModelTransforms* icrb_ModelTransforms = static_cast<ICRB_ModelTransforms*>(inCombinedRenderableBlock->rawSubclassPointer);

		inCombinedRenderableBlock->memBlock = mm_modelTransforms->deallocate(icrb_ModelTransforms->memBlock);

		// The remaping slot is not cleared because it may be reused. If needed it is removed using different logic.
		//icrb_ModelTransforms->memBlock_transformsIndexRemaping->blockEntriesOffsetSideAligned = 0;

		if (icrb_ModelTransforms->inGBucket_toRemoveUnused_listEntry != nullptr) {
			// This must be nullptr when the actual remove occurs.
			throw LogicException(LOC);
		}
	} else {
		// Unhandled provider type.
		throw LogicException(LOC);
	}
}

void CombinedRenderable::releaseTransformsRemapingSlot(ICRB_ModelTransforms* icrb_ModelTransforms) {
	if (icrb_ModelTransforms->memBlock_transformsIndexRemaping == nullptr || icrb_ModelTransforms->memBlock_transformsIndexRemaping->memMan->combinedRenderable != this) {
		throw LogicException(LOC);
	}

	icrb_ModelTransforms->memBlock_transformsIndexRemaping = mm_remapingModelTransformsIndices->deallocate(icrb_ModelTransforms->memBlock_transformsIndexRemaping/*entriesOffset*/);
}

void CombinedRenderable::writeChangedDatas() {
	ListDL<AllocatedMemBlock*>::Entry* entry;

	while ((entry = mm_vertices->dirtyDataBlocksList.getFirst()) != nullptr) {
		AllocatedMemBlock* activeMemBlock = entry->data;
		Entry_GeometryManagerBucket* entryGBucket = activeMemBlock->dataSource_bucketEntry_forVI;
		GeometryProviderInstance* gpi_Vertices = entryGBucket->gpInstancesArray[IGeometryProvider_Vertices::PROVIDER_TYPE].gpInstances;
		
		int insertVerticesCount = static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVerticesCount();

		if ((activeMemBlock->dirtyDataBitMask & AllocatedMemBlock::DirtyDataBitGroups::Main) == AllocatedMemBlock::DirtyDataBitGroups::Main) {
			void* insertVerticesData = static_cast<IGeometryProvider_Vertices*>(gpi_Vertices->customProviderRawPtr)->getVerticesData();

			// Add the vertices to vbo and save the offset.
			void* verticesInsertDest = static_cast<char*>(mm_vertices->data) + activeMemBlock->blockEntriesOffsetSideAligned * mm_vertices->stride;

			memcpy(verticesInsertDest, insertVerticesData, mm_vertices->stride * insertVerticesCount);
			mm_vertices->bufferObject->invalidateRegion(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesCount);
		}
		
		if ((activeMemBlock->dirtyDataBitMask & AllocatedMemBlock::DirtyDataBitGroups::RemapingMTransSlot) == AllocatedMemBlock::DirtyDataBitGroups::RemapingMTransSlot) {
			// Update the vertices to have the required slot.

			// Grab the icrb_ manually and it is guaranteed to exist.
			ICRB_ModelTransforms* icrb_modelTransforms = static_cast<ICRB_ModelTransforms*>(entryGBucket->gpInstancesArray[IGeometryProvider_ModelTransforms::PROVIDER_TYPE].gpInstances->inCombinedRenderableBlockIndexedArray->getDirect(uIndex)->rawSubclassPointer);

			for (int i = 0; i < activeMemBlock->blockEntriesCount; i++) {
				unsigned int* a_modelTransformIndex = reinterpret_cast<unsigned int*>(static_cast<char*>(mm_vertices->data) + (activeMemBlock->blockEntriesOffsetSideAligned + i) * mm_vertices->stride + mm_modelTransforms->vaByteOffset_worldTransformIndex);

				*a_modelTransformIndex = icrb_modelTransforms->memBlock_transformsIndexRemaping->blockEntriesOffsetSideAligned;
			}
			mm_vertices->bufferObject->invalidateRegion(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesCount);
		}
		
		if ((activeMemBlock->dirtyDataBitMask & (AllocatedMemBlock::DirtyDataBitGroups::Main | AllocatedMemBlock::DirtyDataBitGroups::RemapingMTransSlot)) != activeMemBlock->dirtyDataBitMask) {
			// There are unhandled invalidation flags.
			throw LogicException(LOC);
		}

		activeMemBlock->dirtyDataBitMask = 0;
		activeMemBlock->inDirtyDataBlocks_listEntry->remove();
		activeMemBlock->inDirtyDataBlocks_listEntry = nullptr;
	}
	while ((entry = mm_indices->dirtyDataBlocksList.getFirst()) != nullptr) {
		AllocatedMemBlock* activeMemBlock = entry->data;
		Entry_GeometryManagerBucket* entryGBucket = activeMemBlock->dataSource_bucketEntry_forVI;
		GeometryProviderInstance* gpi_Indices = entryGBucket->gpInstancesArray[IGeometryProvider_Indices::PROVIDER_TYPE].gpInstances;
		InCombinedRenderableBlock* icrb_vertices = entryGBucket->inCombinedRenderableProviderBlocks[IGeometryProvider_Vertices::PROVIDER_TYPE];

		if ((activeMemBlock->dirtyDataBitMask & AllocatedMemBlock::DirtyDataBitGroups::Main) == AllocatedMemBlock::DirtyDataBitGroups::Main) {
			int indicesInsertCount = static_cast<IGeometryProvider_Indices*>(gpi_Indices->customProviderRawPtr)->getIndicesCount();

			// Enumerate and add the indices with the vertices offset from above.
			unsigned short* srcBuff = static_cast<unsigned short*>(static_cast<IGeometryProvider_Indices*>(gpi_Indices->customProviderRawPtr)->getIndicesData());
			unsigned short* destBuff = static_cast<unsigned short*>(static_cast<void*>(mm_indices->data));

			for (int i = 0; i < indicesInsertCount; i++) {
				destBuff[activeMemBlock->blockEntriesOffsetSideAligned + i] = icrb_vertices->memBlock->blockEntriesOffsetSideAligned + srcBuff[i];
			}
			mm_indices->bufferObject->invalidateRegion(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesCount);
		}

		if ((activeMemBlock->dirtyDataBitMask & (AllocatedMemBlock::DirtyDataBitGroups::Main)) != activeMemBlock->dirtyDataBitMask) {
			// There are unhandled invalidation flags.
			throw LogicException(LOC);
		}

		activeMemBlock->dirtyDataBitMask = 0;
		activeMemBlock->inDirtyDataBlocks_listEntry->remove();
		activeMemBlock->inDirtyDataBlocks_listEntry = nullptr;
	}
	if (mm_modelTransforms != nullptr) {
		Bounds1DInt invalidVolatileRegion{};

		while ((entry = mm_modelTransforms->dirtyDataBlocksList.getFirst()) != nullptr) {
			AllocatedMemBlock* activeMemBlock = entry->data;
			GeometryProviderInstance* gpi_ModelTransforms = activeMemBlock->dataSource_gpi_forMt;

			if ((activeMemBlock->dirtyDataBitMask & AllocatedMemBlock::DirtyDataBitGroups::Main) == AllocatedMemBlock::DirtyDataBitGroups::Main) {
				{
					void* insertDest = static_cast<char*>(mm_modelTransforms->data) + activeMemBlock->blockEntriesOffsetSideAligned * mm_modelTransforms->stride;

					memcpy(insertDest, static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getWorldTransformMustExist(), mm_modelTransforms->stride * 1/*entries_count*/);

					if (activeMemBlock->memBlockAny->blockMemoryManager == mm_modelTransforms->blockMM_stable) {
						mm_modelTransforms->bufferObject->invalidateRegion(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesCount);
					} else if (activeMemBlock->memBlockAny->blockMemoryManager == mm_modelTransforms->blockMM_volatile) {
						invalidVolatileRegion.unionOther(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesOffsetSideAligned + activeMemBlock->blockEntriesCount);
					} else {
						throw LogicException(LOC);
					}
				}

				if (mm_modelTransforms->bufferObjectB != nullptr) {
					// Write the model_normal_transforms.
					void* insertDest = static_cast<char*>(static_cast<void*>(mm_modelTransforms->dataB)) + activeMemBlock->blockEntriesOffsetSideAligned * mm_modelTransforms->strideB;
					
					memcpy(insertDest, static_cast<IGeometryProvider_ModelTransforms*>(gpi_ModelTransforms->customProviderRawPtr)->getNormalWorldTransformMustExist(), mm_modelTransforms->strideB * 1/*entries_count*/);
					if (activeMemBlock->memBlockAny->blockMemoryManager == mm_modelTransforms->blockMM_stable) {
						mm_modelTransforms->bufferObjectB->invalidateRegion(activeMemBlock->blockEntriesOffsetSideAligned, activeMemBlock->blockEntriesCount);
					}
				}
			}

			if ((activeMemBlock->dirtyDataBitMask & (AllocatedMemBlock::DirtyDataBitGroups::Main)) != activeMemBlock->dirtyDataBitMask) {
				// There are unhandled invalidation flags.
				throw LogicException(LOC);
			}

			activeMemBlock->dirtyDataBitMask = 0;
			activeMemBlock->inDirtyDataBlocks_listEntry->remove();
			activeMemBlock->inDirtyDataBlocks_listEntry = nullptr;
		}

		if (invalidVolatileRegion.getLenX() > 0) {
			mm_modelTransforms->bufferObject->invalidateRegion(invalidVolatileRegion.min.x, invalidVolatileRegion.getLenX());
			mm_modelTransforms->bufferObjectB->invalidateRegion(invalidVolatileRegion.min.x, invalidVolatileRegion.getLenX());
		}
	}
	if (mm_remapingModelTransformsIndices != nullptr) {
		while ((entry = mm_remapingModelTransformsIndices->dirtyDataBlocksList.getFirst()) != nullptr) {
			AllocatedMemBlock* activeMemBlock = entry->data;
			ICRB_ModelTransforms* icrb_modelTransforms = activeMemBlock->dataSource_icrb_forRemapingMt;

			if ((activeMemBlock->dirtyDataBitMask & AllocatedMemBlock::DirtyDataBitGroups::Main) == AllocatedMemBlock::DirtyDataBitGroups::Main) {
				unsigned int directComponentIndex = static_cast<unsigned int>(icrb_modelTransforms->memBlock_transformsIndexRemaping->blockEntriesOffsetSideAligned) / 2u;
				unsigned int vec4ArrayIndex = static_cast<unsigned int>(icrb_modelTransforms->memBlock_transformsIndexRemaping->blockEntriesOffsetSideAligned) / 8u;

				// {0, 1}
				unsigned int subComponentIndex = static_cast<unsigned int>(icrb_modelTransforms->memBlock_transformsIndexRemaping->blockEntriesOffsetSideAligned) - directComponentIndex * 2u;

				// Decode the existing value.
				unsigned short subComponentVals[2];
				// high val
				subComponentVals[1] = static_cast<unsigned int*>(mm_remapingModelTransformsIndices->data)[directComponentIndex] / 65536u;
				// low val
				subComponentVals[0] = static_cast<unsigned int*>(mm_remapingModelTransformsIndices->data)[directComponentIndex] - subComponentVals[1] * 65536u;

				// Update the target subcomponent.
				subComponentVals[subComponentIndex] = static_cast<unsigned int>(icrb_modelTransforms->memBlock->blockEntriesOffsetSideAligned);

				// Extract the other val and re-encode the entire entry.
				// Note. the first val has the lower range of [0, 65536).
				/// The second val uses a multiplier of 65536.
				// Add the 2 values in any order.
				static_cast<unsigned int*>(mm_remapingModelTransformsIndices->data)[directComponentIndex] = subComponentVals[1] * 65536u + subComponentVals[0];
				mm_remapingModelTransformsIndices->bufferObject->invalidateRegion(vec4ArrayIndex, 1);
			}

			if ((activeMemBlock->dirtyDataBitMask & (AllocatedMemBlock::DirtyDataBitGroups::Main)) != activeMemBlock->dirtyDataBitMask) {
				// There are unhandled invalidation flags.
				throw LogicException(LOC);
			}

			activeMemBlock->dirtyDataBitMask = 0;
			activeMemBlock->inDirtyDataBlocks_listEntry->remove();
			activeMemBlock->inDirtyDataBlocks_listEntry = nullptr;
		}
	}
}

int CombinedRenderable::getIndicesCount() {
	return mm_indices->getIndicesCount();
}

void CombinedRenderable::dispose() {
	if (inGlobalList_cEntry == nullptr) {
		throw LogicException(LOC);
	}
	inGlobalList_cEntry->remove();
	inGlobalList_cEntry = nullptr;

	super::dispose();
}

CombinedRenderable::~CombinedRenderable() {
	//void
}
