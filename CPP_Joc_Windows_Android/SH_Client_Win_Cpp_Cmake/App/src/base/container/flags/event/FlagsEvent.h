#pragma once

#include <base/gh.h>
#include <string>

namespace base {
	class FlagEntry;
};

namespace base {
class FlagsEvent {pub dCtor(FlagsEvent);
	priv static int typeCounter;

	pub int type;
	pub std::string name;

	pub base::FlagEntry* flag;

    pub explicit FlagsEvent(std::string name);

	pub void clearData();

	pub FlagsEvent* setData(base::FlagEntry* flag);

	pub virtual ~FlagsEvent();
};
};
