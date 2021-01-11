#include "VH_DebugView.h"
#include <base/view/debug/DebugView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

using namespace base;

VH_DebugView::VH_DebugView(IApp* app)
	: super(app)
{
	//void
}

void VH_DebugView::initDebugView() {
	addComponent(debugView = newt base::DebugView(app), true/*doCreate*/);
	layer2D_debug->addChild(debugView->getVisualsHolder());
	debugView->show(false, nullptr, nullptr);
}

base::IDebugView* VH_DebugView::getDebugView() {
	return debugView;
}

VH_DebugView::~VH_DebugView() {
	//void
}
