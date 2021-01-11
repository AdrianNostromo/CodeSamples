#pragma once

#include <base/gh.h>
#include "Vector3.h"
#include "Quaternion.h"

namespace base {
class PosRotSize3D {
	pub Vector3 pos;
	pub Quaternion rot;
	// The scale may be used as a size if voxel blocks are used.
	pub Vector3 scale;

   pub dCtor(PosRotSize3D);
    pub explicit PosRotSize3D(Vector3& pos, Quaternion& rot, Vector3& scale);

    pub virtual ~PosRotSize3D();
};
};
