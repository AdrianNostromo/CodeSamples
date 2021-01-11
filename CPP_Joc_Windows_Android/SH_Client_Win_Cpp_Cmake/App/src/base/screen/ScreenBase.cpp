#include <base/app/config/AppConfig.h>
#include <base/math/Math.h>
#include <base/exceptions/ValueNotFoundException.h>
#include "ScreenBase.h"
#include "ScreenUtil.h"
#include <base/exceptions/LogicException.h>

using namespace base;

ScreenBase::ScreenBase()
    : super()
{
    ScreenUtil::screen = this;
}

void ScreenBase::setActiveScreenMetrics(
	int width, int height,
	float ppiX, float ppiY)
{
	windowResolution.set(
		width,
		height
	);
	// This was used for layouts in the old version of "View"s. May not be used any more.
	templateSize.set(
		360,
		640
	);

	screenMeasuredDPI.set(
		ppiX,
		ppiY
	);

	computeNormalisedUiDPI();

	//One dp is a virtual pixel unit that's roughly equal to one pixel on a medium-density screen (160dpi; the "baseline" density).
	windowSizeInches.set(
		windowResolution.x / screenMeasuredDPI.x,
		windowResolution.y / screenMeasuredDPI.y
	);
	windowDiagonalInches = Math::dist(
		0.0f, 0.0f,
		windowSizeInches.x, windowSizeInches.y
	);

	// Source: https://material.google.com/patterns/navigation-drawer.html#navigation-drawer-specs
	const float mobileStandardIncrement = 56.0f;
	const float mediumAndLargeStandardIncrement = 64.0f;
	if(getWindowDiagonalInches() < AppConfig::MEDIUM_DEVICE_DIAGONAL_INCHES_MIN) {
		standardIncrement = mobileStandardIncrement * getScreenDensityMultiplier()->x;
	}else {
		standardIncrement = mediumAndLargeStandardIncrement * getScreenDensityMultiplier()->x;
	}

	if (windowResolution.x == 0 || windowResolution.y == 0) {
		windowAspectRatio = 1.0f;
	}
	else {
		windowAspectRatio = windowResolution.x / windowResolution.y;
	}

	isScreenActive = true;

	onActiveScreenMetricsChanged();
}

void ScreenBase::setDisableScreen() {
	isScreenActive = false;

	onScreenDisabled();
}

bool ScreenBase::getIsScreenActive() {
	return isScreenActive;
}

void ScreenBase::computeNormalisedUiDPI() {
	// Modify density to be larger on larger screens. Use the diagonal size for the math.
	// Default density on <=5 inch screens. 50% Larger density on >=11 inch screens.
	windowSizeInches.set(
		windowResolution.x / screenMeasuredDPI.x,
		windowResolution.y / screenMeasuredDPI.y
	);
	float windowDiagonalInches = Math::dist(
		0.0f, 0.0f,
		windowSizeInches.x, windowSizeInches.y
	);
	uiDPI.set(screenMeasuredDPI);

	// The fontsDensityScale values are from tests, modify as needed.
	float fontsDensityScale = 1.3f;
	if(windowDiagonalInches > AppConfig::MEDIUM_DEVICE_DIAGONAL_INCHES_MIN) {
		fontsDensityScale = 1.5f;

		float n;
		if(windowDiagonalInches >= AppConfig::LARGE_DEVICE_DIAGONAL_INCHES_MIN) {
			n = AppConfig::LARGE_DEVICE_DPI_SCALE_MAX;
		}else {
			float cp = 1.0f - ((AppConfig::LARGE_DEVICE_DIAGONAL_INCHES_MIN - windowDiagonalInches) / (AppConfig::LARGE_DEVICE_DIAGONAL_INCHES_MIN - AppConfig::MEDIUM_DEVICE_DIAGONAL_INCHES_MIN));
			n = 1.0f + (AppConfig::LARGE_DEVICE_DPI_SCALE_MAX - 1.0f) * cp;
		}

		uiDPI.mul(n);
	}

	uiDensityMultiplier.set(
		uiDPI.x / 160.0f,
		uiDPI.y / 160.0f
	);
	uiDensityMultiplier_fonts = uiDensityMultiplier.y * fontsDensityScale;
}

float ScreenBase::templateXToScreenX(float templateX) {
	float ret = templateX * uiDensityMultiplier.x;

	return ret;
}

float ScreenBase::templateYToScreenY(float templateY) {
	float ret = templateY * uiDensityMultiplier.y;

	return ret;
}

float ScreenBase::screenXToTemplateX(float screenX) {
	float ret = screenX / uiDensityMultiplier.x;

	return ret;
}

float ScreenBase::screenYToTemplateY(float screenY) {
	float ret = screenY / uiDensityMultiplier.y;

	return ret;
}

Vector2Int* ScreenBase::getWindowResolution() {
	return &windowResolution;
}

int ScreenBase::getWindowResolutionX() {
	if(!isScreenActive) {
		throw LogicException(LOC);
	}

	return windowResolution.x;
}

int ScreenBase::getWindowResolutionY() {
	if(!isScreenActive) {
		throw LogicException(LOC);
	}

	return windowResolution.y;
}

float ScreenBase::getStandardIncrement() {
	return standardIncrement;
}

Vector2* ScreenBase::getWindowSizeInches() {
	return &windowSizeInches;
}

float ScreenBase::getWindowDiagonalInches() {
	return windowDiagonalInches;
}

Vector2* ScreenBase::getScreenMeasuredDPI() {
	return &screenMeasuredDPI;
}

Vector2* ScreenBase::getUiDpi() {
	return &uiDPI;
}

float ScreenBase::getUiDpi_X() {
	return uiDPI.x;
}

float ScreenBase::getUiDpi_Y() {
	return uiDPI.y;
}

Vector2* ScreenBase::getTemplateSize() {
	return &templateSize;
}

float ScreenBase::getWindowAspectRatio() {
	return windowAspectRatio;
}

void ScreenBase::onActiveScreenMetricsChanged() {
	//void
}

void ScreenBase::onScreenDisabled() {
	//void
}

ScreenBase::~ScreenBase() {
    //void
}
