#pragma once

#include <base/gh.h>
#include "TZEA_Tool.h"

namespace rpg3D {
class ToolZEAssets : public TZEA_Tool {priv typedef TZEA_Tool super;pub dCtor(ToolZEAssets);
	
	pub explicit ToolZEAssets(ToolConfigZEAssets* config, std::shared_ptr<ExtraData> extraData);

	pub ~ToolZEAssets() override;
};
};
