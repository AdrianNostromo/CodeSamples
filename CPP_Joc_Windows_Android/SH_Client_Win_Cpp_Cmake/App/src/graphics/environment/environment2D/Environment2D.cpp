#include "Environment2D.h"
#include <graphics/camera/OrthographicCamera.h>
#include <base/screen/ScreenUtil.h>

using namespace graphics;

Environment2D::Environment2D()
	: super()
{
	//void
}

void Environment2D::onColorCameraViewportSizeChanged() {
	super::onColorCameraViewportSizeChanged();

	getCameraMustExist()->setPosition(base::ScreenUtil::screen->getWindowResolutionX() / 2.0f, base::ScreenUtil::screen->getWindowResolutionY() / 2.0f, 0.0f);
}

Environment2D::~Environment2D() {
	//void
}
