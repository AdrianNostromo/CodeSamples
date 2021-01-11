#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

class BoundingBox;

class ITouchable3DModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual BoundingBox* getTouchBoundsAABB() = 0;

    pub ~ITouchable3DModule() override = default;
};
