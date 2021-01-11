#pragma once

#include <base/gh.h>
#include "TZES_Tool.h"

namespace rpg3D {
class ToolZESun : public TZES_Tool {priv typedef TZES_Tool super;pub dCtor(ToolZESun);
	pub explicit ToolZESun(ToolConfigZESun* config, std::shared_ptr<ExtraData> extraData);

	pub ~ToolZESun() override;
};
};
