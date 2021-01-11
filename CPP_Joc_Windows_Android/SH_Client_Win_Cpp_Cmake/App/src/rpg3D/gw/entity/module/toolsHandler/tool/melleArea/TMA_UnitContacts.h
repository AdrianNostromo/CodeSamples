#pragma once

#include <base/gh.h>
#include "TMA_Visual.h"

namespace rpg3D {
class TMA_UnitContacts : public TMA_Visual {priv typedef TMA_Visual super;pub dCtor(TMA_UnitContacts);
    pub explicit TMA_UnitContacts(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData);

    pub ~TMA_UnitContacts() override;
};
};
