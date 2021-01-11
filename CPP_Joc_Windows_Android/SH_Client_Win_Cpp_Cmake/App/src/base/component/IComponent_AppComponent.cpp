#include "IComponent_AppComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_AppComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_AppComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
