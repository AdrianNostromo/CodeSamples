#pragma once

#include <base/gh.h>
#include "TZE_InteractionFilters.h"

namespace rpg3D {
class ToolZoneEditor : public TZE_InteractionFilters {priv typedef TZE_InteractionFilters super;pub dCtor(ToolZoneEditor);
    pub explicit ToolZoneEditor(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

    pub ~ToolZoneEditor() override;
};
};
