#pragma once

#include <base/gh.h>
#include <base/map/Map1D.h>
#include <base/object/IManagedObject.h>

class IComponentLayer;
class IComponent;

class IComponentsHandler : public virtual base::IManagedObject {
	prot virtual void* getComponentLayerVoidP(int type, bool mustExist) = 0;

    pub template <typename T>
	T getComponentAs(bool mustExist);

	pub virtual bool hasComponent(int type) = 0;

	// disposeOrderDefault5 is used on dispose to remove the components in order.
	pub virtual void addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5 =5) = 0;
	pub virtual void removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose=false) = 0;

    pub virtual ~IComponentsHandler() = default;
};

template <typename T>
T IComponentsHandler::getComponentAs(bool mustExist) {
	void* vpComp = getComponentLayerVoidP(std::remove_pointer_t<T>::TYPE, false/*mustExist*/);
	if (vpComp == nullptr) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return nullptr;
	}

	T cComp = static_cast<T>(vpComp);
	
	return cComp;
}
