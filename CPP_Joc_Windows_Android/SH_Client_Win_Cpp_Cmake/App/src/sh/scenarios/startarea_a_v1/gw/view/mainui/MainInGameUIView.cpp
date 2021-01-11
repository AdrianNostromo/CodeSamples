#include "MainInGameUIView.h"
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/exceptions/LogicException.h>
#include <functional>
#include <base/statics/StaticsInit.h>

using namespace startarea_a_v1;

ArrayList<MenuItemConfig*>* MainInGameUIView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
	->appendDirect_chain(super::viewItemConfigs);
});

MainInGameUIView::MainInGameUIView(IApp* app, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewAnimationDurations, gw)
{
    //void
}

MainInGameUIView::~MainInGameUIView() {
    //void
}
