#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include "IUserInputDispatcher.h"
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <memory>

class btDiscreteDynamicsWorld;
namespace rpg3D {
	class WorldOrEntityInteractionEvent;
};

namespace rpg3D {
class IGPUserEntitiesController : virtual public IGamePart, virtual public IUserInputDispatcher {
	priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual int getEntityControlType() = 0;
	pub virtual void setEntityControlType(int entityControlType) = 0;

	pub virtual base::TypedEventDispatcher1D<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>& getGlobalInteractionsDispatcher() = 0;

    pub ~IGPUserEntitiesController() override = default;
};
};
