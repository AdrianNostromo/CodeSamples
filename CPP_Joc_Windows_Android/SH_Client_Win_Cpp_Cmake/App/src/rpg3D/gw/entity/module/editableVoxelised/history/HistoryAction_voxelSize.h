#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class HistoryAction_voxelSize : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub float new_voxelSize;
	pub float old_voxelSize;

public:
   pub dCtor(HistoryAction_voxelSize);
    pub explicit HistoryAction_voxelSize(
		float new_voxelSize, float old_voxelSize);

    pub ~HistoryAction_voxelSize() override;
};
};
