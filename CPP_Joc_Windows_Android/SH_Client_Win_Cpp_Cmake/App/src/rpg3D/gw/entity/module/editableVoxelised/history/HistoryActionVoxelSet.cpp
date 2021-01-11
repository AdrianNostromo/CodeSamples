#include "HistoryActionVoxelSet.h"

using namespace rpg3D;

int HistoryActionVoxelSet::TYPE = getNewTypeId();

HistoryActionVoxelSet::HistoryActionVoxelSet(Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin, int oldEditorMatIndex, int newEditorMatIndex)
	: super(TYPE),
	gPos(gPos), postChangeGridBoundsMin(postChangeGridBoundsMin),
	oldEditorMatIndex(oldEditorMatIndex), newEditorMatIndex(newEditorMatIndex)
{
	rawSubclassPointer = this;
}

HistoryActionVoxelSet::~HistoryActionVoxelSet() {
	//void
}
