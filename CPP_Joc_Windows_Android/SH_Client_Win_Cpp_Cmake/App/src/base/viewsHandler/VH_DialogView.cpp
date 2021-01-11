#include "VH_DialogView.h"
#include <base/view/dialog/DialogView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

using namespace base;

VH_DialogView::VH_DialogView(IApp* app)
	: super(app)
{
	//void
}

base::IDialogView* VH_DialogView::getNewDialogViewAutoDisposable() {
	base::DialogView* newView = newt base::DialogView(app, this);
	addComponent(newView, true/*doCreate*/);

	layerDialogs->addChild(newView->getVisualsHolder());

	return newView;
}

VH_DialogView::~VH_DialogView() {
	//void
}
