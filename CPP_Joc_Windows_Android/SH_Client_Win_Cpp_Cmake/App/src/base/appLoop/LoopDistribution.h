#pragma once

#include "LD_Dispatcher.h"

namespace base {
class LoopDistribution : public LD_Dispatcher {priv typedef LD_Dispatcher super;pub dCtor(LoopDistribution);
	pub explicit LoopDistribution();

	pub ~LoopDistribution() override;
};
};
