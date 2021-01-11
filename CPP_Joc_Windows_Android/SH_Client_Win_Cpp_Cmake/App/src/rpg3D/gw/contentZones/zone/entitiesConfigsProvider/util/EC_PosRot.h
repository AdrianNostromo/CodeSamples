#pragma once

#include <base/gh.h>
#include "EntityConfigBase.h"
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

namespace rpg3D {
class EC_PosRot : public EntityConfigBase {priv typedef EntityConfigBase super;pub dCtor(EC_PosRot);
    pub static const int TYPE;

    pub Vector3 pos;
    pub Quaternion rot;

    pub explicit EC_PosRot(
        std::shared_ptr<std::string> instanceId,
        Vector3& pos, Quaternion& rot);

    pub ~EC_PosRot() override;
};
};
