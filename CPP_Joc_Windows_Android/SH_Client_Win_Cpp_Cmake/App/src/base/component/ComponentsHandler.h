#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IComponentsHandler.h"
#include <base/list/ListDL.h>
#include <memory>

class IComponent;
class IApp;
class IEventListener;
namespace base {
	class LoopEvent;
}

class ComponentsHandler : public base::ManagedObject, public virtual IComponentsHandler {priv typedef base::ManagedObject super;pub dCtor(ComponentsHandler);
	priv int componentCategoryAllowed;
	
	// This is set and used only for the app views comp_handler and gw views comp_handler.
	// If this is set, it allows for components removal with delayed_dispose. This is used to grab a loop.
	// This mechanism allows for views to be delayed dispose from the events they dispatch.
	priv IApp* delayedDisposeSupportApp;
	priv ListDL<IComponent*> delayedDisposeComponentsList{};
	priv std::shared_ptr<IEventListener> delayedDisposeEventListener = nullptr;

    // The values are pointers to the actual interfaces that contain a TYPE and isTypeValid.
    priv Map1D<int, void*> componentsMap{};
	priv ListDL<IComponent*> componentsList{};

	// This is not updated when components are removed but that is not needed because only a small number of components use a different dispose order than the default.
	priv int disposeOrdersUsedBitMap = 0;

	pub explicit ComponentsHandler(int componentCategoryAllowed, IApp* delayedDisposeSupportApp=nullptr);

	prot ListDL<IComponent*>& getComponentsList();

	prot void* getComponentLayerVoidP(int type, bool mustExist) final;

	pub bool hasComponent(int type) final;

	pub void addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5 = 5) override;
	pub void removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose=false) override;

	prot virtual void onLoopEvent_delayedComponentsDispose(IEventListener& eventListener, base::LoopEvent& event);

	prot void dispose() override;
    pub ~ComponentsHandler() override;
};
