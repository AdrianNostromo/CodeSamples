#pragma once

#include <base/gh.h>
#include <memory>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <base/math/PosRotSize3D.h>
#include <base/math/Matrix4.h>

namespace base {
class Visual3DVoxelUtil {
   pub dCtor(Visual3DVoxelUtil);
    pub explicit Visual3DVoxelUtil();

	pub static std::shared_ptr<PosRotSize3D> drawablePositionalToGlobalPositional(Matrix4 srcTransform);
	pub static std::shared_ptr<PosRotSize3D> drawablePositionalToGlobalPositional(
		Matrix4& srcTransform,
		float subScaleX, float subScaleY, float subScaleZ,
		float posX, float posY, float posZ);

    pub virtual ~Visual3DVoxelUtil();
};
};
