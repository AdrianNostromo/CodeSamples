#pragma once

#include <base/gh.h>
#include <base/list/Array1D.h>
#include <base/math/Vector3.h>
#include <base/math/Vector3Int.h>
#include <base/math/Matrix4.h>
#include <base/math/collision/Ray.h>
#include <memory>

namespace rpg3D {
class VoxelSideTouchDetectionHelper {
	priv class SideConfig {
		pub int sideId;
		pub std::shared_ptr<Array1D<float>> sideTemplateVoxels;
	
		pub dCtor(SideConfig);
		pub SideConfig(int sideId, std::shared_ptr<Array1D<float>> sideTemplateVoxels)
			: sideId(sideId), sideTemplateVoxels(sideTemplateVoxels)
		{
			//void
		}
	};

	priv static Array1D<SideConfig>* sideConfigsList;

public:
   pub dCtor(VoxelSideTouchDetectionHelper);
    pub explicit VoxelSideTouchDetectionHelper();

	pub static int computeTouchSide(
		float voxelSize, Vector3& voxelGridOffset, Matrix4& parentTransform, Ray& ray,
		Vector3Int& testVoxelGPos);

    pub virtual ~VoxelSideTouchDetectionHelper();
};
};
