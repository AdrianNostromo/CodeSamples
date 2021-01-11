#pragma once

#include <base/gh.h>
#include "GM_Buckets.h"
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/list/Array1D.h>

namespace graphics {
    class CombinedRenderable;
};

namespace graphics {
class GM_Validation : public GM_Buckets {priv typedef GM_Buckets super;pub dCtor(GM_Validation);
    pub explicit GM_Validation();

    pub void tickLogic() final;

    pub ~GM_Validation() override;
};
};
