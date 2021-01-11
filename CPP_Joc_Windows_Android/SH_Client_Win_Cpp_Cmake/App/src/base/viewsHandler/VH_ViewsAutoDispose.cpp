#include "VH_ViewsAutoDispose.h"

using namespace base;

VH_ViewsAutoDispose::VH_ViewsAutoDispose(IApp* app)
	: super(app)
{
	//void
}

void VH_ViewsAutoDispose::onRequestDisposeView(IComponent* target) {
	// Place it in a list and remove it in a loop tick because the target memory will get deleted while it still runs code.
	removeComponent(target, true/*doDispose*/, true/*useDelayedDispose*/);
}

VH_ViewsAutoDispose::~VH_ViewsAutoDispose() {
	//void
}
