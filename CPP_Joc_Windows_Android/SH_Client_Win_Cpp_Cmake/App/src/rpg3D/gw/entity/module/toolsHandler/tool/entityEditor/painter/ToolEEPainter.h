#pragma once

#include <base/gh.h>
#include "TEEPa_Tool.h"

namespace rpg3D {
class ToolEEPainter : public TEEPa_Tool {priv typedef TEEPa_Tool super;pub dCtor(ToolEEPainter);
	
	pub explicit ToolEEPainter(ToolConfigEEPainter* config, std::shared_ptr<ExtraData> extraData);

	pub ~ToolEEPainter() override;
};
};
