#include "HistoryAction_voxelSize.h"

using namespace rpg3D;
int HistoryAction_voxelSize::TYPE = getNewTypeId();

HistoryAction_voxelSize::HistoryAction_voxelSize(
	float new_voxelSize, float old_voxelSize)
	: super(TYPE),
	new_voxelSize(new_voxelSize), old_voxelSize(old_voxelSize)
{
	rawSubclassPointer = this;
}

HistoryAction_voxelSize::~HistoryAction_voxelSize() {
	//void
}
