#include "DynamicTextButtonConfig.h"

using namespace base;

DynamicTextButtonConfig::DynamicTextButtonConfig(
	int id,
	std::string text, Color& tintColor, float alpha)
	: id(id),
	text(text), tintColor(tintColor), alpha(alpha)
{
	//void
}

DynamicTextButtonConfig::~DynamicTextButtonConfig() {
	//void
}
