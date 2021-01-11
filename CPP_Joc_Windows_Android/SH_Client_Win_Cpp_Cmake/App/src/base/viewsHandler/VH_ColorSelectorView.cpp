#include "VH_ColorSelectorView.h"
#include <base/view/colorSelector/ColorSelectorView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

using namespace base;

VH_ColorSelectorView::VH_ColorSelectorView(IApp* app)
	: super(app)
{
	//void
}

base::IColorSelectorView* VH_ColorSelectorView::getNewColorSelectorViewAutoDisposable() {
	base::ColorSelectorView* newView = newt base::ColorSelectorView(app, this);
	addComponent(newView, true/*doCreate*/);

	layerDialogs->addChild(newView->getVisualsHolder());

	return newView;
}

VH_ColorSelectorView::~VH_ColorSelectorView() {
	//void
}
