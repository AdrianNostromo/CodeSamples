#pragma once

#include <base/gh.h>
#include "GPP3D_Bullet.h"

namespace rpg3D {
class GamePartPhysics3D : public GPP3D_Bullet {priv typedef GPP3D_Bullet super;pub dCtor(GamePartPhysics3D);
    pub explicit GamePartPhysics3D(base::IGameWorld* gw);

    pub ~GamePartPhysics3D() override;
};
};
