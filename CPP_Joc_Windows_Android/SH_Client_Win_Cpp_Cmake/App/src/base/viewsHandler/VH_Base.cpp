#include "VH_Base.h"
#include <base/component/IComponent_UIView.h>

using namespace base;

VH_Base::VH_Base(IApp* app)
	: super(IComponent_UIView::COMPONENT_CATEGORY, app/*delayedDisposeSupportApp*/),
	app(app)
{
	//void
}

VH_Base::~VH_Base() {
	//void
}
