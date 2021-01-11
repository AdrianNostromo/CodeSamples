#pragma once

#include <base/gh.h>
#include "TZEWFT_Tool.h"

namespace rpg3D {
class ToolZEWallsFullTiled : public TZEWFT_Tool {priv typedef TZEWFT_Tool super;pub dCtor(ToolZEWallsFullTiled);
	pub explicit ToolZEWallsFullTiled(ToolConfigZEWallsFullTiled* config, std::shared_ptr<ExtraData> extraData);

	pub ~ToolZEWallsFullTiled() override;
};
};
