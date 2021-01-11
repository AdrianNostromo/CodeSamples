#pragma once

#include <base/gh.h>
#include "../IRenderPhase.h"

namespace graphics {
class IRenderPhase2DFinalOutput : virtual public IRenderPhase {
	pub static int TYPE;
	pub int getType() final;

    pub virtual ~IRenderPhase2DFinalOutput() = default;
};
};
