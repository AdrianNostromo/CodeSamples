#pragma once

#include <base/gh.h>
#include "T_ExtraActivationSecondaryControls.h"
#include <base/list/Array1D.h>
#include <base/list/ArrayList.h>
#include <memory>

namespace rpg3D {
class Tool : public T_ExtraActivationSecondaryControls {priv typedef T_ExtraActivationSecondaryControls super;pub dCtor(Tool);
	pub explicit Tool(ToolConfigBase* config);

	pub ~Tool() override;
};
};
