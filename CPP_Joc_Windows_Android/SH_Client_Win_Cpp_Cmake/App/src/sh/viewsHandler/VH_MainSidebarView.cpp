#include "VH_MainSidebarView.h"
#include <sh/view/mainsidebar/MainSidebarView.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <sh/app/IAppSquareHeads.h>

using namespace sh;

VH_MainSidebarView::VH_MainSidebarView(IAppSquareHeads* app)
	: super(app)
{
	//void
}

IMainSidebarView* VH_MainSidebarView::getOrCreateMainSidebarView() {
	if (mainSidebarView == nullptr) {
		addComponent(mainSidebarView = new MainSidebarView(app, app, app), true/*doCreate*/);
		layerSidebar->addChild(mainSidebarView->getVisualsHolder());
	}

	return mainSidebarView;
}

VH_MainSidebarView::~VH_MainSidebarView() {
	//void
}
