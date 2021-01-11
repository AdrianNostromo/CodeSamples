#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/customLogic/CustomLogicModule.h>
#include "ICustomLogicModule_PaintPad.h"
#include <worldGame3D/gw/event/GWEvent.h>

class IDrawableNode3D;
namespace rpg3D {
	class IPlatformModule;
}

namespace rpg3D {
class CustomLogicModule_PaintPad : public base::CustomLogicModule, public virtual ICustomLogicModule_PaintPad {priv typedef base::CustomLogicModule super;pub dCtor(CustomLogicModule_PaintPad);
    priv std::string brushId;
    priv std::string brushHolderId;

    priv IDrawableNode3D* brush = nullptr;
    priv IDrawableNode3D* brushHolder = nullptr;

    priv float brushAngZ;
    priv float brushAngX = 60.0f;

    priv IPlatformModule* platform = nullptr;

    // This is used to make the initial update and update the tick when animationCompletionStatus reaches 1.0f;
    priv bool isStateDirty = false;

    priv bool currentActiveStatus = false;

    pub explicit CustomLogicModule_PaintPad(
        IWorldEntity* entity,
		base::CustomLogicTemplate* t,
        std::string brushId, std::string brushHolderId);
    prot void createBMain() override;

    priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

    pub ~CustomLogicModule_PaintPad() override;
};
};
