#include "VH_SplashScreenView.h"
#include <base/view/splashScreen/SplashScreenView.h>

using namespace base;

VH_SplashScreenView::VH_SplashScreenView(IApp* app)
	: super(app)
{
	//void
}

base::SplashScreenView* VH_SplashScreenView::getSplashScreenView() {
    if (splashScreenView == nullptr) {
        addComponent(splashScreenView = newt base::SplashScreenView(app, this), true/*doCreate*/);
        layer2D_splashScreen->addChild(splashScreenView->getVisualsHolder());
    }

    return splashScreenView;
}

void VH_SplashScreenView::hideAndAutoDisposeSplashScreen() {
    getSplashScreenView()->hide(true, nullptr);
    // View will auto-dispose on hide finish, just remove the pointer.
    splashScreenView = nullptr;
}

VH_SplashScreenView::~VH_SplashScreenView() {
	//void
}
