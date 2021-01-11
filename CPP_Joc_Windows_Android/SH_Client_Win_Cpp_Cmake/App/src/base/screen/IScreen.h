#pragma once

#include <functional>
#include <base/gh.h>
#include <string>
#include <base/math/Vector2.h>
#include <base/math/Vector2Int.h>
#include <base/object/ManagedListener.h>

namespace base {
class IScreen {
	pub class IListener : virtual public IManagedListener {
		pub virtual void onActiveScreenMetricsChange(
			int resolutionX, int resolutionY,
			float ppiX, float ppiY) = 0;

		pub virtual void onActiveScreenResolutionChange(
			int resolutionX, int resolutionY) = 0;

		pub virtual void onScreenDisabled() = 0;

		pub ~IListener() override = default;
	};
	pub class CbListener : public ManagedListener, virtual public IListener {priv typedef ManagedListener super;pub dCtor(CbListener);
		pub std::string codeLocation;

		pub std::function<void(
			int resolutionX, int resolutionY,
			float ppiX, float ppiY)> cb_onActiveScreenMetricsChange = nullptr;

		pub std::function<void(
			int resolutionX, int resolutionY)> cb_onActiveScreenResolutionChange = nullptr;

		pub std::function<void()> cb_onScreenDisabled = nullptr;

		pub explicit CbListener();
		pub explicit CbListener(std::string codeLocation);

		pub void onActiveScreenMetricsChange(
			int resolutionX, int resolutionY,
			float ppiX, float ppiY) override;

		pub void onActiveScreenResolutionChange(
			int resolutionX, int resolutionY) override;

		pub void onScreenDisabled() override;

		pub ~CbListener() override;
	};

	pub virtual void addListener(IListener* listener, int logLocationId) = 0;
	pub virtual void removeListener(IListener* listener, int logLocationId) = 0;

	pub virtual bool getIsScreenActive() = 0;

	pub virtual float templateXToScreenX(float templateX) = 0;
	pub virtual float templateYToScreenY(float templateY) = 0;
	pub virtual float screenXToTemplateX(float screenX) = 0;
	pub virtual float screenYToTemplateY(float screenY) = 0;

	pub virtual Vector2Int* getWindowResolution() = 0;
	pub virtual int getWindowResolutionX() = 0;
	pub virtual int getWindowResolutionY() = 0;

	pub virtual float getStandardIncrement() = 0;

	pub virtual Vector2* getWindowSizeInches() = 0;
	pub virtual float getWindowDiagonalInches() = 0;

	pub virtual Vector2* getScreenMeasuredDPI() = 0;

	pub virtual Vector2* getUiDpi() = 0;
	pub virtual float getUiDpi_X() = 0;
	pub virtual float getUiDpi_Y() = 0;

	pub virtual Vector2* getTemplateSize() = 0;

	pub virtual inline Vector2* getScreenDensityMultiplier() = 0;
	pub virtual inline float getScreenDensityMultiplier_fonts() = 0;

	pub virtual float getWindowAspectRatio() = 0;

	pub virtual ~IScreen() = default;
};
};
