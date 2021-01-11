#pragma once

#include <base/object/ManagedObject.h>
#include <base/math/Vector2.h>
#include <base/math/Vector2Int.h>
#include "IScreen.h"

namespace base {
class ScreenBase : public ManagedObject, virtual public IScreen {
private: typedef ManagedObject super;
private:
	bool isScreenActive = false;

	// This is initialize on app create.
	// Use float for the screen size. It is valid.
	Vector2Int windowResolution{0, 0};
	Vector2 windowSizeInches{0.0f, 0.0f};
	float windowDiagonalInches = 0.0f;
	Vector2 templateSize{0.0f, 0.0f};
	// Source: https://material.google.com/patterns/navigation-drawer.html#navigation-drawer-specs
	float standardIncrement = 0.0f;

	// This is used for logic only (eg: Camera 3d zoom, device type). For visuals use uiDPI.
	Vector2 screenMeasuredDPI{0.0f, 0.0f};

	// This is used for user interface and other things.
	Vector2 uiDPI{0.0f, 0.0f};
	// This is used to preserve real world sizes across different device screens. Also includes scaling up for large devices (eg. monitors).
	Vector2 uiDensityMultiplier{0.0f, 0.0f};
	float uiDensityMultiplier_fonts = 0.0f;

	// resolution_x / resolution_y
	float windowAspectRatio;

public:
    ScreenBase(ScreenBase const&) = delete;
    ScreenBase(ScreenBase &&) = default;
    ScreenBase& operator=(ScreenBase const&) = delete;
    ScreenBase& operator=(ScreenBase &&) = default;

    explicit ScreenBase();

	void setActiveScreenMetrics(
		int width, int height,
		float ppiX, float ppiY);
	void setDisableScreen();

	bool getIsScreenActive() override;

	float templateXToScreenX(float templateX) override;
	float templateYToScreenY(float templateY) override;
	float screenXToTemplateX(float screenX) override;
	float screenYToTemplateY(float screenY) override;

	Vector2Int* getWindowResolution() override;
	int getWindowResolutionX() override;
	int getWindowResolutionY() override;

	float getStandardIncrement() override;

	Vector2* getWindowSizeInches() override;
	float getWindowDiagonalInches() override;

	Vector2* getScreenMeasuredDPI() override;

	Vector2* getUiDpi() override;
	float getUiDpi_X() override;
	float getUiDpi_Y() override;

	Vector2* getTemplateSize() override;

	// Making this virtual function final may allow it to be inlined (Not sure if true. If false, the function will not be inlined because it is virtual).
	inline Vector2* getScreenDensityMultiplier() final;
	inline float getScreenDensityMultiplier_fonts() final;

	float getWindowAspectRatio() override;

	~ScreenBase() override;
protected:
	virtual void onActiveScreenMetricsChanged();
	virtual void onScreenDisabled();

private:
	void computeNormalisedUiDPI();

};

inline Vector2* ScreenBase::getScreenDensityMultiplier() {
	return &uiDensityMultiplier;
}

inline float ScreenBase::getScreenDensityMultiplier_fonts() {
	return uiDensityMultiplier_fonts;
}

};
