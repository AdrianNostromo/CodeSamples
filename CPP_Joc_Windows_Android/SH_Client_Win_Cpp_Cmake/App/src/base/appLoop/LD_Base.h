#pragma once

#include "ILoopDistribution.h"

namespace base {
class LD_Base : virtual public ILoopDistribution {pub dCtor(LD_Base);
	pub explicit LD_Base();
	
	pub ~LD_Base() override;
};
};
