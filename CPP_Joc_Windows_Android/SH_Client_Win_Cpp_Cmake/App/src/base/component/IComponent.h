#pragma once

#include <base/gh.h>
#include <base/map/Map1D.h>
#include <base/object/IManagedObject.h>

#define DEC_componentBasics()\
pub void updateInHandlerMapEntry(Map1D<int, void*>& componentsMap, bool operationType) override;

#define DEF_componentBasics(CLASS)\
void CLASS::updateInHandlerMapEntry(Map1D<int, void*>& componentsMap, bool operationType) {\
	super::updateInHandlerMapEntry(componentsMap, operationType);\
\
	if (!operationType) {\
		if (componentsMap.removeAndGetDirect(TYPE) != this) {\
			throw LogicException(LOC);\
		}\
	} else {\
		componentsMap.putDirect(TYPE, this);\
	}\
}

class IListEntry;

// NOTE. If multiple interfaces in a class_stack use this IComponent system, the interfaces must extend each other (as a list) and call super as needed.
/// This is a requirement or some interfaces pointers won't be registered in the handler.
class IComponent : public virtual base::IManagedObject {
	prot static int GetNew_TYPE();
	prot static int GetNew_COMPONENT_CATEGORY();
	pub IListEntry* inComponentsHandler_listEntry = nullptr;
	pub IListEntry* inComponentsHandler_delayedRemovalListEntry = nullptr;
	pub int disposeOrder = 0;
	pub virtual void updateInHandlerMapEntry(Map1D<int, void*>& componentsMap, bool operationType);
	// This is used to avoid adding a component to the wrong components_handler.
	pub virtual int getComponentCategory() = 0;

	// This is used to access the component category class (not interface) if needed (eg: GamePart) to call extra code (eg. createB, disposePrePre, disposePre).
	// Eg. GamePart*, EntityModule*;
	pub void* componentClassVoidP = nullptr;

    pub virtual ~IComponent() = default;
};
