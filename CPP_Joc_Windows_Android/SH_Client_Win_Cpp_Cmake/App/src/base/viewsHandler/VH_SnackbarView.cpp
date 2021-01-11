#include "VH_SnackbarView.h"
#include <base/view/snackbar/SnackbarView.h>
#include <base/view/snackbar/util/WrappedSnackbarView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/view/snackbar/inOrderDisplay/SnackbarViewsInOrderDisplayHandler.h>

using namespace base;

VH_SnackbarView::VH_SnackbarView(IApp* app)
	: super(app)
{
	//void
}

sp<base::snackbar::WrappedSnackbarView> VH_SnackbarView::getNewSnackbarViewAutoDisposable() {
	base::snackbar::SnackbarView* newView = newt base::snackbar::SnackbarView(app, this, snackbarViewsInOrderDisplayHandler);
	addComponent(newView, true/*doCreate*/);

	layerDialogs->addChild(newView->getVisualsHolder());

	return newView->getWrapper();
}

VH_SnackbarView::~VH_SnackbarView() {
	//void
}
