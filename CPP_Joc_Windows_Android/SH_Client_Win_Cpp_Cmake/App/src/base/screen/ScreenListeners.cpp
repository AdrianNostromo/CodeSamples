#include "ScreenListeners.h"
#include <base/exceptions/LogicException.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
 
using namespace base;

const bool ScreenListeners::IS_DEBUG_LISTENERS = false;

ScreenListeners::ScreenListeners()
	: super()
{
	//void
}

void ScreenListeners::addListener(IListener* listener, int logLocationId) {
	if (AppConfig::IS_DEBUG && IS_DEBUG_LISTENERS) {
		GlobalAccessLogHandler::devLogHandler->post_debug("base::ScreenListeners::addListener %i", logLocationId);
	}

	if(listener->getIsListening()) {
		throw LogicException(LOC);
	}
	listener->setIsListening(true);

	if(screenListenersList.contains(listener)) {
		throw LogicException(LOC);
	}

	screenListenersList.appendDirect(listener);
}

void ScreenListeners::removeListener(IListener* listener, int logLocationId) {
	if (AppConfig::IS_DEBUG && IS_DEBUG_LISTENERS) {
		GlobalAccessLogHandler::devLogHandler->post_debug("base::ScreenListeners::removeListener %i", logLocationId);
	}

	if(!listener->getIsListening()) {
		throw LogicException(LOC);
	}
	listener->setIsListening(false);

	if(!screenListenersList.remove(listener)) {
		throw LogicException(LOC);
	}
}

void ScreenListeners::onActiveScreenMetricsChanged() {
	super::onActiveScreenMetricsChanged();

	dispatchEvent_onActiveScreenMetricsChange(
		getWindowResolutionX(), getWindowResolutionY(),
		getUiDpi_X(), getUiDpi_Y()
	);
	dispatchEvent_onActiveScreenResolutionChange(
		getWindowResolutionX(), getWindowResolutionY()
	);
}

void ScreenListeners::onScreenDisabled() {
	super::onScreenDisabled();

	dispatchEvent_onScreenDisabled();
}

void ScreenListeners::dispatchEvent_onActiveScreenMetricsChange(
	int resolutionX, int resolutionY,
	float ppiX, float ppiY)
{
	for(int i=0;i<screenListenersList.size();i++) {
		IListener* entry = screenListenersList.getDirect(i);

		entry->onActiveScreenMetricsChange(
			resolutionX, resolutionY,
			ppiX, ppiY
		);
	}
}

void ScreenListeners::dispatchEvent_onActiveScreenResolutionChange(
	int resolutionX, int resolutionY)
{
	for(int i=0;i<screenListenersList.size();i++) {
		IListener* entry = screenListenersList.getDirect(i);

		entry->onActiveScreenResolutionChange(resolutionX, resolutionY);
	}
}

void ScreenListeners::dispatchEvent_onScreenDisabled() {
	for(int i=0;i<screenListenersList.size();i++) {
		IListener* entry = screenListenersList.getDirect(i);

		entry->onScreenDisabled();
	}
}

void ScreenListeners::disposeMain() {
	if(screenListenersList.size() > 0) {
		throw LogicException(LOC);
	}

	super::disposeMain();
}

ScreenListeners::~ScreenListeners() {
	//vod
}
