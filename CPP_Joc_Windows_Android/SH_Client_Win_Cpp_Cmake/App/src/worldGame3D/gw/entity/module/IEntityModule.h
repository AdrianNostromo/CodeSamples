#pragma once

#include <base/gh.h>
#include <base/object/IManagedObject.h>
#include <base/component/IComponent_GWWorldEntityComponent.h>
#include <string>

class IWorldEntity;
namespace worldGame3D {
	class IGameWorldGameParts;
};

class IEntityModule : virtual public base::IManagedObject, public virtual IComponent_GWWorldEntityComponent {
	pub static int ActionGroup_Stoppable;
	
	prot static int GetNew_ID();
	// This is used for error detection when calling entity->getComponentAs(...);
	// This must be extended by interfaces that implement the ID;
	pub virtual int getId();

	pub virtual std::string getModuleId() = 0;

	pub virtual IWorldEntity* getEntity() = 0;

	pub virtual void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) = 0;
	pub virtual void disconnectFromGamePartsPre() = 0;
	
	pub virtual void reservedCreateB() = 0;
	pub virtual void reservedDisposePre() = 0;

	pub ~IEntityModule() override = default;
};
