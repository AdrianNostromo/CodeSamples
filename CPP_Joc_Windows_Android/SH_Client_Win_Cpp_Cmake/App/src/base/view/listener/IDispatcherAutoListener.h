#pragma once

#include <base/gh.h>

namespace base {
	class IAutoListener;
};

namespace base {
class IDispatcherAutoListener {
	pub virtual void onAutoListenerDisposing(IAutoListener* listener) = 0;

    pub virtual ~IDispatcherAutoListener() = default;
};
};
