#include "ComponentsHandler.h"
#include "IComponent.h"
#include <base/exceptions/LogicException.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/app/IApp.h>
#include <base/appLoop/event/LoopEvent.h>

ComponentsHandler::ComponentsHandler(int componentCategoryAllowed, IApp* delayedDisposeSupportApp)
	: super(),
	componentCategoryAllowed(componentCategoryAllowed),
	delayedDisposeSupportApp(delayedDisposeSupportApp)
{
	//void
}

ListDL<IComponent*>& ComponentsHandler::getComponentsList() {
	return componentsList;
}

void* ComponentsHandler::getComponentLayerVoidP(int type, bool mustExist) {
	void* vpComp = componentsMap.getDirect(type, nullptr);
	if (vpComp == nullptr) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return nullptr;
	}

	return vpComp;
}

bool ComponentsHandler::hasComponent(int type) {
	void* vpComp = componentsMap.getDirect(type, nullptr);
	if (vpComp == nullptr) {
		return false;
	}

	return true;
}

void ComponentsHandler::addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5) {
	if (component->inComponentsHandler_listEntry != nullptr || disposeOrderDefault5 < 0 || disposeOrderDefault5 >= 32
		|| component->getComponentCategory() != componentCategoryAllowed)
	{
		int cat = component->getComponentCategory();
		throw LogicException(LOC);
	}
	
	component->inComponentsHandler_listEntry = componentsList.appendDirect(component);
	component->disposeOrder = disposeOrderDefault5;
	disposeOrdersUsedBitMap |= 1 << disposeOrderDefault5;
	component->updateInHandlerMapEntry(componentsMap, true/*operationType*/);
	
	if (doCreate) {
		component->reservedCreate();
	}
}

void ComponentsHandler::removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose) {
	if (component->inComponentsHandler_listEntry == nullptr 
		|| component->inComponentsHandler_delayedRemovalListEntry != nullptr)
	{
		throw LogicException(LOC);
	}

	if (doDispose) {
		if (useDelayedDispose) {
			if (delayedDisposeSupportApp == nullptr) {
				// Must set the optional constructor param to enable this functionality.
				throw LogicException(LOC);
			}
			component->inComponentsHandler_delayedRemovalListEntry = delayedDisposeComponentsList.appendDirect(component);

			if (delayedDisposeComponentsList.count() == 1) {
				// Add the loop listener.
				delayedDisposeEventListener = delayedDisposeSupportApp->getLoopDistribution()->addEventListener(
					base::ILoopDistribution::AppLoopEvent_delayedComponentsDispose->type,
					std::bind(&ComponentsHandler::onLoopEvent_delayedComponentsDispose, this, std::placeholders::_1, std::placeholders::_2)
				);
			}

			return;
		}

		component->reservedDispose();
	}

	component->updateInHandlerMapEntry(componentsMap, false/*operationType*/);

	component->inComponentsHandler_listEntry->remove();
	component->inComponentsHandler_listEntry = nullptr;

	if (doDispose) {
		delete component;
	}
}

void ComponentsHandler::onLoopEvent_delayedComponentsDispose(IEventListener& eventListener, base::LoopEvent& event) {
	while (delayedDisposeComponentsList.count() > 0) {
		ListDL<IComponent*>::Entry* listEntry = delayedDisposeComponentsList.getFirst();
		IComponent* comp = listEntry->data;

		listEntry->data->inComponentsHandler_delayedRemovalListEntry = nullptr;
		listEntry->remove();

		removeComponent(comp, true/*doDispose*/, false/*useDelayedDispose*/);
	}

	if (delayedDisposeComponentsList.count() == 0) {
		// Remove the loop listener.
		delayedDisposeEventListener = nullptr;
	}
}

void ComponentsHandler::dispose() {
	// These entries are no used because all entries get disposed now.
	while (delayedDisposeComponentsList.count() > 0) {
		ListDL<IComponent*>::Entry* listEntry = delayedDisposeComponentsList.getFirst();
		// This is required to avoid a error detected in the below removeComponent call.
		listEntry->data->inComponentsHandler_delayedRemovalListEntry = nullptr;
		listEntry->remove();
	}

	int cDisposeOrder = -1;
	while (componentsList.count() > 0) {
		cDisposeOrder++;
		if (cDisposeOrder >= 32) {
			// Bug probably occured.
			throw LogicException(LOC);
		}
		if ((disposeOrdersUsedBitMap & (1 << cDisposeOrder)) != (1 << cDisposeOrder)) {
			// Dispose order layer not used.
			continue;
		}

		componentsList.enumerateDirect([this, cDisposeOrder](ListDL<IComponent*>::Entry* listEntry, IComponent*& data, bool& stopEnumeration) ->void {
			if (data->disposeOrder == cDisposeOrder) {
				removeComponent(data, true/*doDispose*/, false/*useDelayedDispose*/);
			}
		});
	}

	super::dispose();
}

ComponentsHandler::~ComponentsHandler() {
	//void
}
