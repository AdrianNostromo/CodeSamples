#pragma once

#include <base/gh.h>

class EL_Base;

class IEventDispatcher {
	pub virtual void removeListener(EL_Base& eventListener) = 0;

	pub virtual ~IEventDispatcher() = default;
};
