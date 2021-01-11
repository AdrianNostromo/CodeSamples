#include "IScreen.h"

using namespace base;

IScreen::CbListener::CbListener()
	: super(),
	codeLocation("")
{
	//void
}

IScreen::CbListener::CbListener(std::string codeLocation)
	: super(),
	codeLocation(codeLocation)
{
	//void
}

void IScreen::CbListener::onActiveScreenMetricsChange(
	int resolutionX, int resolutionY,
	float ppiX, float ppiY)
{
	if(cb_onActiveScreenMetricsChange) {
		cb_onActiveScreenMetricsChange(
			resolutionX, resolutionY,
			ppiX, ppiY
		);
	}
}

void IScreen::CbListener::onActiveScreenResolutionChange(
	int resolutionX, int resolutionY)
{
	if(cb_onActiveScreenResolutionChange) {
		cb_onActiveScreenResolutionChange(
			resolutionX, resolutionY
		);
	}
}

void IScreen::CbListener::onScreenDisabled() {
	if(cb_onScreenDisabled) {
		cb_onScreenDisabled();
	}
}

IScreen::CbListener::~CbListener() {
	//void
}
