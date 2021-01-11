#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "ILookAtModule.h"
#include <worldGame3D/gw/entity/template/lookAt/LookAtTemplate.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

class IWorldEntity;

class LookAtModule : public EntityModule, public virtual ILookAtModule {priv typedef EntityModule super;pub dCtor(LookAtModule);
    pub LookAtTemplate* t;

    // This value is used to set the first person camera direction.
    // This value is set by the entity controller when using mouse or device tilt for unit direction control.
    //asdA9;// update this as needed;
    pub Vector3 lookAtDirection{};

    pub float angDElevation = 0.0f;
    pub float angDZ = 0.0f;

    pub Quaternion rotElevation{};
    pub Quaternion rotZ{};
    pub Quaternion rotCombined{};

    pub explicit LookAtModule(
        IWorldEntity* entity,
        LookAtTemplate* t
    );

    pub Vector3& peekLookAtDirection() final;

    pub float getAngDElevation() final;
    pub float getAngDZ() final;

    pub void setLookAtAnglesDeg(float angDZ, float angDElevation) final;

    pub ~LookAtModule() override;
};
