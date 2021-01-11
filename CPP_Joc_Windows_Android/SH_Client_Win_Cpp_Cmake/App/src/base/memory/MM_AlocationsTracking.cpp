#include "MM_AlocationsTracking.h"
#include <base/log/GlobalAccessLogHandler.h>

using namespace mem;

MM_AlocationsTracking::MM_AlocationsTracking()
	: super()
{
	//void
}

void MM_AlocationsTracking::trackAllocation(void* ptr, unsigned int byteCount, char allocationType, const char* fileName, size_t lineIndex) {
	std::pair<const char*, size_t> key( fileName, lineIndex );

	stats.ram_bytes_count += byteCount;
	stats.activeAllocations_bytes_count += byteCount;

	TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* locationIndicator = locationIndicatorsTree.getNode(key);
	if (locationIndicator == nullptr) {
		locationIndicator = locationIndicatorsTree.insertEmplace(key, fileName, lineIndex);
	}

	locationIndicator->value.allocationsCount++;

	// Insert the pointer tracking entry;
	allocationsTree.insertEmplace(ptr, ptr, byteCount, allocationType, locationIndicator);
}

const char* MM_AlocationsTracking::untrackAllocation(void* block, char allocationType) {
	TreeAVLKeyValue<void*, AllocationInformation>::Node* allocationInformation = allocationsTree.getNode(block);
	if (allocationInformation == nullptr) {
		// Not allocated.
		throw LogicException(LOC);
	}

	stats.ram_bytes_count -= allocationInformation->value.bytesCount;
	stats.activeAllocations_bytes_count -= allocationInformation->value.bytesCount;

	// Decrement the locations counter and remove the entry if 0 reached.
	if (allocationInformation->value.allocationSource == nullptr || allocationInformation->value.allocationType != allocationType || allocationInformation->value.allocationSource->value.allocationsCount <= 0) {
		throw LogicException(LOC);
	}

	const char* fileName = allocationInformation->value.allocationSource->value.fileName;

	allocationInformation->value.allocationSource->value.allocationsCount--;
	if (allocationInformation->value.allocationSource->value.allocationsCount == 0) {
		allocationInformation->value.allocationSource->remove();
	}
	allocationInformation->value.allocationSource = nullptr;

	allocationInformation->remove();

	return fileName;
}

void MM_AlocationsTracking::startDeltaAllocationsSnapshot() {
	locationIndicatorsTree.enumerateInOrderDirect([](TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* node, LocationIndicator& value, bool& stopEnumeration) ->void {
		value.memLeaksDetectionSnapshot_allocationsCount = value.allocationsCount;
	});
}

void MM_AlocationsTracking::logLargestDeltaAllocations() {
	TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* largestAllocationsNode_nullptr = nullptr;
	TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* largestAllocationsNode_nonNullptr = nullptr;
	locationIndicatorsTree.enumerateInOrderDirect([&largestAllocationsNode_nullptr, &largestAllocationsNode_nonNullptr](TreeAVLKeyValue<std::pair<const char*, size_t>, LocationIndicator>::Node* node, LocationIndicator& value, bool& stopEnumeration) ->void {
		unsigned int deltaAllocationsCount = value.allocationsCount - value.memLeaksDetectionSnapshot_allocationsCount;

		if (deltaAllocationsCount > 0) {
			if (value.fileName == nullptr) {
				if (largestAllocationsNode_nullptr == nullptr || deltaAllocationsCount > (largestAllocationsNode_nullptr->value.allocationsCount - largestAllocationsNode_nullptr->value.memLeaksDetectionSnapshot_allocationsCount)) {
					largestAllocationsNode_nullptr = node;
				}
			} else {
				if (largestAllocationsNode_nonNullptr == nullptr || deltaAllocationsCount > (largestAllocationsNode_nonNullptr->value.allocationsCount - largestAllocationsNode_nonNullptr->value.memLeaksDetectionSnapshot_allocationsCount)) {
					largestAllocationsNode_nonNullptr = node;
				}
			}
		}
	});

	if (largestAllocationsNode_nullptr != nullptr) {
		GlobalAccessLogHandler::devLogHandler->post_debug("MM Largest nullptr allocation: deltaCount: %i, fileName: %s, lineINdex: %i", largestAllocationsNode_nullptr->value.allocationsCount - largestAllocationsNode_nullptr->value.memLeaksDetectionSnapshot_allocationsCount, largestAllocationsNode_nullptr->value.fileName, largestAllocationsNode_nullptr->value.lineIndex);
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("MM Largest nullptr allocation: none");
	}
	if (largestAllocationsNode_nonNullptr != nullptr) {
		GlobalAccessLogHandler::devLogHandler->post_debug("MM Largest nonNullptr allocation: deltaCount: %i, fileName: %s, lineINdex: %i", largestAllocationsNode_nonNullptr->value.allocationsCount - largestAllocationsNode_nonNullptr->value.memLeaksDetectionSnapshot_allocationsCount, largestAllocationsNode_nonNullptr->value.fileName, largestAllocationsNode_nonNullptr->value.lineIndex);
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("MM Largest nonNullptr allocation: none");
	}
}

MM_AlocationsTracking::~MM_AlocationsTracking() {
	//void
}
