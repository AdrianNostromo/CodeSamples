#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>
#include <memory>
#include <string>

namespace rpg3D {
class IPlatformModule : public virtual IEntityModule {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual bool getIsActive() = 0;

    pub virtual std::shared_ptr<std::string> getActivation_script() = 0;
    pub virtual void setActivation_script(std::shared_ptr<std::string> activation_script) = 0;

    pub ~IPlatformModule() override = default;
};
};
