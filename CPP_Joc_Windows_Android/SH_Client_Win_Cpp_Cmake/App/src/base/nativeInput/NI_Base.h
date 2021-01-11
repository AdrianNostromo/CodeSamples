#pragma once

#include <base/gh.h>
#include "INativeInput.h"
#include <base/math/Vector2Int.h>

namespace base {
class NI_Base : virtual public INativeInput {pub dCtor(NI_Base);
	prot Vector2Int screenSize{};

    pub explicit NI_Base();

	pub void setScreenSize(int width, int height);

	pub bool getIsHapticFeedbackSupported() override;
	pub void doHapticFeedback(int hapticFeedbackType) override;

    pub ~NI_Base() override;
};
};
