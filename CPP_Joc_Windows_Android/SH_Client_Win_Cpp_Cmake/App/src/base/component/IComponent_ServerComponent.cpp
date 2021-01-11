#include "IComponent_ServerComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_ServerComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_ServerComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
