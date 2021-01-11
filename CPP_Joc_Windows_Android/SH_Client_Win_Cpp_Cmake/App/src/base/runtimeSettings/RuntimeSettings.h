#pragma once

#include <base/gh.h>
#include "RS_Settings.h"

namespace base {
class RuntimeSettings : public RS_Settings {priv typedef RS_Settings super;pub dCtor(RuntimeSettings);
    pub explicit RuntimeSettings();

    pub ~RuntimeSettings() override;
};
};
