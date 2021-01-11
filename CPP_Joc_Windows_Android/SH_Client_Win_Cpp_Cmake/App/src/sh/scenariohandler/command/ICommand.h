#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>

namespace sh {
    class SH_SquareHeads;
};

namespace sh {
class ICommand {
    pub virtual void run(SH_SquareHeads* scenariosHandler, Vector3& sourceTeleporterOffset) = 0;

    pub virtual ~ICommand() = default;
};
};
