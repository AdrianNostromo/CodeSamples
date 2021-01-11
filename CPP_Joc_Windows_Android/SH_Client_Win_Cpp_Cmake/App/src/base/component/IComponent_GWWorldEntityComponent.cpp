#include "IComponent_GWWorldEntityComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_GWWorldEntityComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_GWWorldEntityComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
