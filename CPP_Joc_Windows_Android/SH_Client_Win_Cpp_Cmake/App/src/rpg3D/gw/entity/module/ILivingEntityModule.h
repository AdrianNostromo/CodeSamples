#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

namespace rpg3D {
    class LivingEntityTemplate;
};

namespace rpg3D {
class ILivingEntityModule : public virtual IEntityModule {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual LivingEntityTemplate* getT() = 0;
    pub virtual LivingEntityTemplate* getTemplate() = 0;

    pub virtual float getCurrentHP() = 0;
    pub virtual void setCurrentHP(float currentHP) = 0;
    pub virtual void addCurrentHP(float hpAddAmount) = 0;

    pub virtual bool getRemoveEntityFromGWOnZeroHp() = 0;
    pub virtual void setRemoveEntityFromGWOnZeroHp(bool removeEntityFromGWOnZeroHp) = 0;

    pub ~ILivingEntityModule() override = default;
};
};
