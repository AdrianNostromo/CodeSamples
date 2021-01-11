#pragma once

#include "base/gh.h"
#include "ScreenBase.h"
#include <base/list/ArrayList.h>

namespace base {
class ScreenListeners : public ScreenBase {priv typedef ScreenBase super;pub dCtor(ScreenListeners);
	priv static const bool IS_DEBUG_LISTENERS;
	
	priv ArrayList<IListener*> screenListenersList{};

	pub explicit ScreenListeners();

	pub void addListener(IListener* listener, int logLocationId) override;
	pub void removeListener(IListener* listener, int logLocationId) override;

	prot void onActiveScreenMetricsChanged() override;
	prot void onScreenDisabled() override;

	prot void disposeMain() override;

	priv void dispatchEvent_onActiveScreenMetricsChange(
		int width, int height,
		float ppiX, float ppiY);
	priv void dispatchEvent_onActiveScreenResolutionChange(
		int width, int height);
	priv void dispatchEvent_onScreenDisabled();

	pub ~ScreenListeners() override;
};
};
