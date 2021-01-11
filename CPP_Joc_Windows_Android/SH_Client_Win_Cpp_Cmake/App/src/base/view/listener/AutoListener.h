#pragma once

#include <base/gh.h>
#include "IAutoListener.h"
#include <base/list/ArrayList.h>

namespace base {
class AutoListener : virtual public IAutoListener {pub dCtor(AutoListener);
	priv IDispatcherAutoListener* dispatcher = nullptr;

    pub explicit AutoListener();

	pub IDispatcherAutoListener* getDispatcher() final;
	pub void setDispatcher(IDispatcherAutoListener* dispatcher) final;

    pub ~AutoListener() override;
};
};
