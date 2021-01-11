#include "GLBCTIndividualRegions.h"
#include <base/opengl/util/GLUtil.h>
#include <base/list/CompositesBuffer.h>
#include <base/opengl/IGL.h>
#include <base/math/Math.h>
#include <base/math/Bounds1DInt.h>

using namespace graphics;

GLBCTIndividualRegions::EntriesBlock::EntriesBlock(int blockEntriesOffset, int blockEntriesCount)
	: blockEntriesOffset(blockEntriesOffset), blockEntriesCount(blockEntriesCount)
{
	//void
}

bool GLBCTIndividualRegions::EntriesBlock::operator==(EntriesBlock const& other) noexcept {
	return blockEntriesOffset == other.blockEntriesOffset;
}

bool GLBCTIndividualRegions::EntriesBlock::operator!=(EntriesBlock const& other) noexcept {
	return blockEntriesOffset != other.blockEntriesOffset;
}

bool GLBCTIndividualRegions::EntriesBlock::operator>(EntriesBlock const& other) noexcept {
	return blockEntriesOffset > other.blockEntriesOffset;
}

bool GLBCTIndividualRegions::EntriesBlock::operator<(EntriesBlock const& other) noexcept {
	return blockEntriesOffset < other.blockEntriesOffset;
}

int GLBCTIndividualRegions::ComparatorOverlappOrContactNeighbour::operator()(EntriesBlock& o) const {
	if (targetBlockEntriesOffset + targetBlockEntriesCount < o.blockEntriesOffset) {
		// The key is on the left and not in contact with the tree entry.
		return -1;
	} else if (targetBlockEntriesOffset > o.blockEntriesOffset + o.blockEntriesCount) {
		// The key is on the right and not in contact with the tree entry.
		return 1;
	}

	// Key range is inside a tree entry or is in contact with it. Do more checks below
	int boundsMin = targetBlockEntriesOffset >= o.blockEntriesOffset ? targetBlockEntriesOffset : o.blockEntriesOffset;
	int boundsMax = targetBlockEntriesOffset + targetBlockEntriesCount <= o.blockEntriesOffset + o.blockEntriesCount ? targetBlockEntriesOffset + targetBlockEntriesCount : o.blockEntriesOffset + o.blockEntriesCount;
	if (boundsMax > boundsMin) {
		// The region overlapps with the key.
		return 0;
	}

	// The key is in neighbour_only contact. There is no overlapp.
	return 0;
}

GLBCTIndividualRegions::GLBCTIndividualRegions(CompositesBuffer* buffer)
	: super(buffer)
{
	//void
}

void GLBCTIndividualRegions::validate(GLenum glBindTarget) {
	if (invalidEntryRegionsTree.count() > 0) {
		invalidEntryRegionsTree.enumerateInOrderDirect([this, glBindTarget](TreeAVL<EntriesBlock>::Node* node, EntriesBlock& value, bool& stopEnumeration)->void {
			int stride = buffer->stride();

			GLUtil::gl->bufferSubData(
				glBindTarget/*target*/,
				value.blockEntriesOffset * stride/*offset_in_bytes*/, value.blockEntriesCount * stride/*size_in_bytes*/,
				buffer->getDataBlockAsCharP() + (value.blockEntriesOffset * stride)/*data*/
			);
		});

		invalidEntryRegionsTree.clear();
	}
}

void GLBCTIndividualRegions::invalidateUsedRegion() {
	// If there is a buffer empty center region, invalidate only the sides if they exist.
	if (buffer->getUnusedCenterRegionRef().getLenX() > 0) {
		// The buffer contains a center region that is unused. Update only the sides.
		/// The empty center region is used by mem managers logic.
		int unusedCenterRegion_offsetBytes = buffer->getUnusedCenterRegionRef().min.x * buffer->stride();
		int unusedCenterRegion_countBytes = buffer->getUnusedCenterRegionRef().getLenX() * buffer->stride();

		if (buffer->getUnusedCenterRegionRef().min.x > 0) {
			// Write the left region from start_of_the_buffer to the unused_region.
			invalidateRegion(0, buffer->getUnusedCenterRegionRef().min.x);
		}

		if (buffer->count() > buffer->getUnusedCenterRegionRef().max.x) {
			// Write the left region from unused_region to the buffer end.
			invalidateRegion(buffer->getUnusedCenterRegionRef().max.x, buffer->count() - buffer->getUnusedCenterRegionRef().max.x);
		}
	} else {
		invalidateRegion(0, buffer->count());
	}
}

void GLBCTIndividualRegions::invalidateRegion(int blockEntriesOffset, int blockEntriesCount) {
	if (blockEntriesCount == 0) {
		// Ignore empty block invalidations.
		return;
	}

	// Add a empty fragmented entry to the tree (combine with an existing entry if possible, check for a attach_node).
	// The insert may make it possible to connect 2 already existing entries.

	// Use neighbour find to find the first neighbour if any or do a insert in a single call.
	comparatorOverlappOrContactNeighbour.targetBlockEntriesOffset = blockEntriesOffset;
	comparatorOverlappOrContactNeighbour.targetBlockEntriesCount = blockEntriesCount;
	// This find will return a node with the exact key range if a new entry got connected and there are no neighbours.
	bool isNewInsertOccured = false;
	TreeAVL<EntriesBlock>::Node* invalidEntriesNode = invalidEntryRegionsTree.getNodeOrInsertEmplaceNew(comparatorOverlappOrContactNeighbour, &isNewInsertOccured, blockEntriesOffset, blockEntriesCount);
	if (!isNewInsertOccured) {
		// A existing node found.

		// Add the range to the existing node.
		Bounds1DInt bounds{};
		bounds.unionOther(invalidEntriesNode->data.blockEntriesOffset, invalidEntriesNode->data.blockEntriesOffset + invalidEntriesNode->data.blockEntriesCount);
		bounds.unionOther(blockEntriesOffset, blockEntriesOffset + blockEntriesCount);

		invalidEntriesNode->data.blockEntriesOffset = bounds.min.x;
		invalidEntriesNode->data.blockEntriesCount = bounds.getLenX();

		// Check left and right entries and combine all that are connected into invalidEntriesNode;
		TreeAVL<EntriesBlock>::Node* neighbourEntryNode = nullptr;
		while ((neighbourEntryNode = invalidEntriesNode->getInOrderPreviousNode()) != nullptr 
			&& neighbourEntryNode->data.blockEntriesOffset + neighbourEntryNode->data.blockEntriesCount >= invalidEntriesNode->data.blockEntriesOffset)
		{
			// In contact or overlapping the neighbour.

			//asd_01;// use the new range and do union directly;
			bounds.clear();
			bounds.unionOther(invalidEntriesNode->data.blockEntriesOffset, invalidEntriesNode->data.blockEntriesOffset + invalidEntriesNode->data.blockEntriesCount);
			bounds.unionOther(neighbourEntryNode->data.blockEntriesOffset, neighbourEntryNode->data.blockEntriesOffset + neighbourEntryNode->data.blockEntriesCount);

			invalidEntriesNode->data.blockEntriesOffset = bounds.min.x;
			invalidEntriesNode->data.blockEntriesCount = bounds.getLenX();

			neighbourEntryNode->remove();
		}
		while ((neighbourEntryNode = invalidEntriesNode->getInOrderNextNode()) != nullptr 
			&& invalidEntriesNode->data.blockEntriesOffset + invalidEntriesNode->data.blockEntriesCount >= neighbourEntryNode->data.blockEntriesOffset)
		{
			// In contact or overlapping the neighbour.

			bounds.clear();
			bounds.unionOther(invalidEntriesNode->data.blockEntriesOffset, invalidEntriesNode->data.blockEntriesOffset + invalidEntriesNode->data.blockEntriesCount);
			bounds.unionOther(neighbourEntryNode->data.blockEntriesOffset, neighbourEntryNode->data.blockEntriesOffset + neighbourEntryNode->data.blockEntriesCount);

			invalidEntriesNode->data.blockEntriesOffset = bounds.min.x;
			invalidEntriesNode->data.blockEntriesCount = bounds.getLenX();

			neighbourEntryNode->remove();
		}
	}
}

GLBCTIndividualRegions::~GLBCTIndividualRegions() {
	//void
}
