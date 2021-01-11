#include "IComponent_UIView.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_UIView::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_UIView::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
