#include "IComponent.h"

int IComponent::GetNew_TYPE() {
	static int STATIC_COUNTER = 0;

	int ret = STATIC_COUNTER;
	STATIC_COUNTER++;

	return ret;
}

int IComponent::GetNew_COMPONENT_CATEGORY() {
	static int STATIC_COUNTER = 0;

	int ret = STATIC_COUNTER;
	STATIC_COUNTER++;

	return ret;
}

void IComponent::updateInHandlerMapEntry(Map1D<int, void*>& componentsMap, bool operationType) {
	//void
}
