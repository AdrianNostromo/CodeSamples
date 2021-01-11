#include "IComponent_GameWorldComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_GameWorldComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_GameWorldComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
