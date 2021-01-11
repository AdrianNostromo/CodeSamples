#include "Config.h"
#include <base/view/snackbar/util/DynamicTextButtonConfig.h>

using namespace base;
using namespace base::snackbar;

Config::Config(
	std::string text, float autoCloseTimerS,
	sp<DynamicTextButtonConfig> dynamicButtonConfig)
	: text(text), autoCloseTimerS(autoCloseTimerS),
	dynamicButtonConfig(dynamicButtonConfig)
{
	//void
}

Config::~Config() {
	//void
}
