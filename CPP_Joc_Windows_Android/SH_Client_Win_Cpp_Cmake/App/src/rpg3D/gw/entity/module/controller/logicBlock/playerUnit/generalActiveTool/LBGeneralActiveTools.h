#pragma once

#include <base/gh.h>
#include "LBGAT_SelectedTools.h"

namespace rpg3D {
namespace playerUnit {
class LBGeneralActiveTools : public LBGAT_SelectedTools {priv typedef LBGAT_SelectedTools super; pub dCtor(LBGeneralActiveTools);
    pub explicit LBGeneralActiveTools();

    pub ~LBGeneralActiveTools() override;
};
};
};
