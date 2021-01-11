#pragma once

#include <base/gh.h>
#include "ICommand.h"

namespace sh {
class Cmd_Base : public virtual ICommand {priv typedef ICommand super;pub dCtor(Cmd_Base);
    pub explicit Cmd_Base();

    pub ~Cmd_Base() override;
};
};
