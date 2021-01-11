#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include "StatusView.h"
#include <base/statics/StaticsInit.h>

using namespace sh;

ArrayList<MenuItemConfig*>* StatusView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
    ->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
        ->put("width", 1.0f, "_appSize.width")
        ->put("height", 1.0f, "_appSize.height")
    ))

    ->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
        //void
    ))

    ->appendDirect_chain(new_viewItemConfig_jitConsoleScreenTa())
    ->appendDirect_chain(new_viewItemConfig_jitConsoleScreenDimmer())
    ->appendDirect_chain(new_viewItemConfig_console())
    ->appendDirect_chain(new_viewItemConfig_flasher())
    ->appendDirect_chain(new_viewItemConfig_indicator());
});

ArrayList<StateChangeDurations*>* StatusView::viewEaseDurationsSList = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewEaseDurationsSList = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_COMPLEX_DURATION_S));
});

StatusView::StatusView(
    IAppSquareHeads* app)
    : super(app, viewItemConfigs, viewEaseDurationsSList)
{
    //void
}

StatusView::~StatusView() {
    //void
}
