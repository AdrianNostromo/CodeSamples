#pragma once

#include <base/gh.h>
#include "WE_Base.h"

class WorldEntity : public worldGame3D::WE_Base {priv typedef worldGame3D::WE_Base super;pub dCtor(WorldEntity);
    pub explicit WorldEntity(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData);

    pub ~WorldEntity() override;
};
