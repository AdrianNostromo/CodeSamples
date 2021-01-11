#pragma once

#include <base/gh.h>
#include "RM_List.h"

namespace graphics {
class RenderablesManager : public RM_List {priv typedef RM_List super;pub dCtor(RenderablesManager);
    pub explicit RenderablesManager();

    pub ~RenderablesManager() override;
};
};
