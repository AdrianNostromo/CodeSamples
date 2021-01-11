#pragma once

#include <base/gh.h>
#include "IGameWorldGameParts.h"
#include "IGameWorldBase.h"

// This is used by entities.

class IHandler_GameWorld;
namespace base {
	class EntityEvent;
};

namespace worldGame3D {
class IGameWorldGeneral : public virtual IGameWorldGameParts, public virtual IGameWorldBase {
	pub virtual IHandler_GameWorld* getHandler() = 0;
	
	// This is here and not in IGameWorldBubbledEntityEvents because it is called by entities and this class is accessible by entities.
	pub virtual int dispatchEntityBubbledEvent(base::EntityEvent& event) = 0;

    pub ~IGameWorldGeneral() override = default;
};
};
