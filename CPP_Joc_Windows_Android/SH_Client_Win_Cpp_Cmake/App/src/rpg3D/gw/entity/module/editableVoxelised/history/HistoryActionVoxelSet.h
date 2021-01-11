#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <base/math/Vector3Int.h>

namespace rpg3D {
class HistoryActionVoxelSet : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub Vector3Int gPos;
	pub Vector3Int postChangeGridBoundsMin;
	pub int oldEditorMatIndex;
	pub int newEditorMatIndex;

public:
   pub dCtor(HistoryActionVoxelSet);
    pub explicit HistoryActionVoxelSet(Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin, int oldEditorMatIndex, int newEditorMatIndex);

    pub ~HistoryActionVoxelSet() override;
};
};
