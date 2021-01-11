#pragma once

#include <base/gh.h>
#include <base/object/IObject.h>

class IWorldEntity;
namespace rpg3D {
	class IControllerModule;
};
namespace worldGame3D {
	class IGameWorldGameParts;
};

namespace rpg3D {
class ILogicBlock : virtual public base::IObject {
	prot static int GetNew_ID();
	// This is used for error detection when calling entity->getComponentAs(...);
	// This must be extended by interfaces that implement the ID;
	pub virtual int getId() = 0;
	
	prot static int GetNew_GroupID();
	pub virtual int getGroupID() = 0;

	pub virtual void setEntity(IWorldEntity* entity, rpg3D::IControllerModule* controller) = 0;

	pub virtual void logicTickIfNeeded(float deltaS) = 0;

	pub virtual bool getIsActive() = 0;
	pub virtual void setIsActive(bool isActive, worldGame3D::IGameWorldGameParts* gwGameParts=nullptr) = 0;

	pub virtual void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) = 0;
	pub virtual void disconnectFromGamePartsPre() = 0;

	pub virtual void reservedDisposePre() = 0;

	pub ~ILogicBlock() override = default;
};
};
