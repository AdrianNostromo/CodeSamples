#pragma once

#include <base/gh.h>
#include <string>
#include <base/buffer/Buffer.h>

namespace base {
class AsyncFHEvent {pub dCtor(AsyncFHEvent);
	priv static int typeCounter;
	
	pub int type;
	std::string name;

	pub std::shared_ptr<std::string> data_string;
	pub base::Buffer data_binary;

    pub explicit AsyncFHEvent(std::string name);

	void clearData();

    pub virtual ~AsyncFHEvent();
};
};
