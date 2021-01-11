#include "DeV_Base.h"
#include <base/app/IApp.h>
#include <base/opengl/util/GLUtil.h>

using namespace base;

DeV_Base::DeV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
	app(app)
{
	//void
}

DeV_Base::~DeV_Base() {
	//void
}
