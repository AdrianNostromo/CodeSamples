#pragma once

#include <base/gh.h>

namespace base {
	class IDispatcherAutoListener;
};

namespace base {
class IAutoListener {
	pub virtual IDispatcherAutoListener* getDispatcher() = 0;
	pub virtual void setDispatcher(IDispatcherAutoListener* dispatcher) = 0;

    pub virtual ~IAutoListener() = default;
};
};
