#pragma once

#include <base/gh.h>
#include <base/sp.h>
#include <string>

namespace base {
namespace snackbar {
	class DynamicTextButtonConfig;
};
};

namespace base {
namespace snackbar {
class Config {pub dCtor(Config);
	pub std::string text;

	// Recommended value [4s, 10s];
	pub float autoCloseTimerS;

	pub sp<DynamicTextButtonConfig> dynamicButtonConfig;

	pub explicit Config(
		std::string text, float autoCloseTimerS,
		sp<DynamicTextButtonConfig> dynamicButtonConfig);

	pub virtual ~Config();
};
};
};
