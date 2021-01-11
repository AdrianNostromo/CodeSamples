#include "EA_Base.h"
#include <base/math/util/BoolUtil.h>
#include <string>

using namespace worldGame3D;

int EA_Base::NewTypeIndex() {
	static int TYPE_COUNTER = 0;

	TYPE_COUNTER++;
	return TYPE_COUNTER;
}

int EA_Base::GetNextAvailableGroupSingleBitMap() {
	static int bitIndexCounter = 0;
	if (bitIndexCounter >= 32) {
		// Bits limit reached, use a long or array of longs.
		throw LogicException(LOC);
	}

	int ret = BoolUtil::setBitAtIndexDirect(0, bitIndexCounter);

	bitIndexCounter++;

	return ret;
}

EA_Base::EA_Base(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int localEntityId, int syncedEntityId,
	int priority, 
	int groupsBitMap,
	int preRequirementsGroupsBitMask,
	int replaceGroupsBitMask,
	int overridesGroupsBitMask)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		priority, 
		groupsBitMap,
		preRequirementsGroupsBitMask,
		replaceGroupsBitMask,
		overridesGroupsBitMask),
	localEntityId(localEntityId), syncedEntityId(syncedEntityId)
{
	//void
}

EA_Base::~EA_Base() {
	//void
}
