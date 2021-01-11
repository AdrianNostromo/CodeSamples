#include "UIV_Base.h"
#include <base/app/IApp.h>

UIV_Base::UIV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
    : super(app, app, viewItemConfigs, viewEaseDurationsSList),
    app(app)
{
//void
}

UIV_Base::~UIV_Base() {
    //void
}
