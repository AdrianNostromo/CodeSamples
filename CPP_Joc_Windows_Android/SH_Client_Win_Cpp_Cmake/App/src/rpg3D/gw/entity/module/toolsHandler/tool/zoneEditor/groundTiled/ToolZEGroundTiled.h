#pragma once

#include <base/gh.h>
#include "TZEGT_Tool.h"

namespace rpg3D {
class ToolZEGroundTiled : public TZEGT_Tool {priv typedef TZEGT_Tool super;pub dCtor(ToolZEGroundTiled);
	
	pub explicit ToolZEGroundTiled(ToolConfigZEGroundTiled* config, std::shared_ptr<ExtraData> extraData);

	pub ~ToolZEGroundTiled() override;
};
};
