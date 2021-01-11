#pragma once

#include <base/gh.h>
#include <string>

namespace base {
class AppEvent {pub dCtor(AppEvent);
	priv static int typeCounter;

	pub int type;

	pub std::string name;

	pub explicit AppEvent(std::string name);

	pub void clearData();

	pub virtual ~AppEvent();
};
};
