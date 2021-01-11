#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IGeometryManager.h"
#include <base/list/ListDL.h>

namespace graphics {
    class SharedGeometryManagerData;
};

namespace graphics {
class GM_Base : public base::ManagedObject, public virtual IGeometryManager {priv typedef base::ManagedObject super;pub dCtor(GM_Base);
    pub explicit GM_Base();

    prot void dispose() override;
    pub ~GM_Base() override;
};
};
