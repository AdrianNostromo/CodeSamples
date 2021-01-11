#include "IComponent_ScenariosHandlerComponent.h"
#include <base/exceptions/LogicException.h>
#include "ComponentsHandler.h"

int IComponent_ScenariosHandlerComponent::COMPONENT_CATEGORY = GetNew_COMPONENT_CATEGORY();
int IComponent_ScenariosHandlerComponent::getComponentCategory() {
	return COMPONENT_CATEGORY;
}
