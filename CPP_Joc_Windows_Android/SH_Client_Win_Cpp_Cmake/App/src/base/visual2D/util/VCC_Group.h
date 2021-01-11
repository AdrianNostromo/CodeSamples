#pragma once

#include "VisualCreateConfig.h"

class VCC_Group : public VisualCreateConfig {priv typedef VisualCreateConfig super;pub dCtor(VCC_Group);
    pub static int TYPE;

    pub explicit VCC_Group();

    pub static VCC_Group* New();

    pub virtual ~VCC_Group();
};
