#pragma once

#include <base/gh.h>
#include "EntityConfigBase.h"
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>

namespace graphics {
    class TilingInformation_GridXY;
};

namespace rpg3D {
class EC_TiledXY : public EntityConfigBase {priv typedef EntityConfigBase super;pub dCtor(EC_TiledXY);
    pub static const int TYPE;

    pub Vector3 pos;
    pub Quaternion rot;

    pub std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation;

    pub explicit EC_TiledXY(
        std::shared_ptr<std::string> instanceId,
        Vector3& pos, Quaternion& rot,
        std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation);

    pub ~EC_TiledXY() override;
};
};
