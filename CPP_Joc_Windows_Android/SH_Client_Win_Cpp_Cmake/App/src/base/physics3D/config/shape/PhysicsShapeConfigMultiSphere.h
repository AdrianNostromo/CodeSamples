#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/list/Array1D.h>
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigMultiSphere : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigMultiSphere);
	pub static int TYPE;

	pub class SphereInfo {
		pub Vector3 position;
		pub float radius;

		pub explicit SphereInfo(Vector3 position, float radius);
	};

	pub Array1D<SphereInfo> sphereInfos;
    
	pub explicit PhysicsShapeConfigMultiSphere(int numSpheres, SphereInfo* sphereInfos);

    pub ~PhysicsShapeConfigMultiSphere() override;
};
};
