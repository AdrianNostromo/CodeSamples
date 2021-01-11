#include "DynamicTextButtonConfig.h"

using namespace base;
using namespace base::snackbar;

DynamicTextButtonConfig::DynamicTextButtonConfig(
	int id,
	std::string text, Color& tintColor, float alpha,
	bool hasExclusiveLine)
	: id(id),
	text(text), tintColor(tintColor), alpha(alpha),
	hasExclusiveLine(hasExclusiveLine)
{
	//void
}

DynamicTextButtonConfig::~DynamicTextButtonConfig() {
	//void
}
