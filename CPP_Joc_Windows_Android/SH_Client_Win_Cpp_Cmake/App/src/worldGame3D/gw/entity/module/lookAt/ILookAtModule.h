#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

class ILookAtModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

    pub virtual Vector3& peekLookAtDirection() = 0;

    pub virtual float getAngDElevation() = 0;
    pub virtual float getAngDZ() = 0;

    pub virtual void setLookAtAnglesDeg(float angDZ, float angDElevation) = 0;

    pub ~ILookAtModule() override = default;
};
