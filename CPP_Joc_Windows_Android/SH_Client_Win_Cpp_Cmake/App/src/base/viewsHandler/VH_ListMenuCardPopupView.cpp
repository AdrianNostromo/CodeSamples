#include "VH_ListMenuCardPopupView.h"
#include <base/view/listmenucardpopup/ListMenuCardPopupView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

using namespace base;

VH_ListMenuCardPopupView::VH_ListMenuCardPopupView(IApp* app)
	: super(app)
{
	//void
}

base::IListMenuCardPopupView* VH_ListMenuCardPopupView::getNewListMenuCardPopupViewAutoDisposable(IContainer2D* parentHolderOverride) {
	base::ListMenuCardPopupView* newView = newt base::ListMenuCardPopupView(app, this);
	addComponent(newView, true/*doCreate*/);

	if (parentHolderOverride != nullptr) {
		parentHolderOverride->addChild(newView->getVisualsHolder());
	} else {
		layerDialogs->addChild(newView->getVisualsHolder());
	}

	return newView;
}

VH_ListMenuCardPopupView::~VH_ListMenuCardPopupView() {
	//void
}
