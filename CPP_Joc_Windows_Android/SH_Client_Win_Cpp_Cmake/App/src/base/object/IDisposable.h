#pragma once

#include <base/gh.h>

namespace base {
class IDisposable {
	pub virtual bool getIsDisposed() = 0;

	// Note. call only one.
	pub virtual void reservedDisposeIfNeeded() = 0;
	pub virtual void reservedDispose() = 0;
	pub virtual void reservedDisposeMain() = 0;

	pub virtual ~IDisposable() = default;
};
};
