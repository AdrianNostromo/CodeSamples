#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IContentHandler.h"

class IApp;

namespace base {
namespace contentHandler {
class CH_Base : public base::ManagedObject, public virtual IContentHandler {priv typedef base::ManagedObject super;pub dCtor(CH_Base);
    prot IApp* app;

    pub explicit CH_Base(IApp* app);

    pub ~CH_Base() override;
};
};
};
