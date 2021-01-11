#include "IComponent_ClientAccountHandlerComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_ClientAccountHandlerComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_ClientAccountHandlerComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
