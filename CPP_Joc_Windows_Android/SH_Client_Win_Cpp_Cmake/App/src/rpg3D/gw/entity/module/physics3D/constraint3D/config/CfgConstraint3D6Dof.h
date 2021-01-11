#pragma once

#include <base/gh.h>
#include "CfgConstraint3D.h"
#include <LinearMath/btTransform.h>

namespace rpg3D {
ATTRIBUTE_ALIGNED16(class) CfgConstraint3D6Dof : public CfgConstraint3D {priv typedef CfgConstraint3D super;pub dCtor(CfgConstraint3D6Dof);
	pub static int TYPE;

	btTransform offsetInActorA;
	btTransform offsetInActorB;

	bool useLinearReferenceFrameA;
    
	BT_DECLARE_ALIGNED_ALLOCATOR();
	pub explicit CfgConstraint3D6Dof(
		btTransform& offsetInActorA, btTransform& offsetInActorB,
		bool useLinearReferenceFrameA);

    pub ~CfgConstraint3D6Dof() override;
};
};
