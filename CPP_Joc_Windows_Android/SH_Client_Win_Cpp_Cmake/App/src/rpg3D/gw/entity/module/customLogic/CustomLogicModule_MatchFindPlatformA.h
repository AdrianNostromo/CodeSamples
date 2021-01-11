#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/customLogic/CustomLogicModule.h>
#include "ICustomLogicModule_MatchFindPlatformA.h"
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <worldGame3D/gw/event/GWEvent.h>

namespace rpg3D {
class IPlatformModule;

class CustomLogicModule_MatchFindPlatformA : public base::CustomLogicModule, public virtual ICustomLogicModule_MatchFindPlatformA {priv typedef base::CustomLogicModule super;pub dCtor(CustomLogicModule_MatchFindPlatformA);
    priv static const float z_IDDLE;
    priv static const float z_ACTIVE;

    priv static const float rotDeg_IDDLE;
    priv static const float rotDeg_ACTIVE;
    priv static const Vector3 rotAxis;

    priv std::string spinnerId;
    priv std::string spinnerSymbolId;

    priv IDrawableNode3D* spinner = nullptr;
    priv IDrawableNode3D* spinnerSymbol = nullptr;

    priv bool currentActiveStatus = false;
    priv float animationCompletionStatus = 1.0f;
    // This is used to make the initial update and update the tick when animationCompletionStatus reaches 1.0f;
    priv bool isStateDirty = false;

    priv bool nextActiveState = false;

    priv IPlatformModule* platform = nullptr;

    pub explicit CustomLogicModule_MatchFindPlatformA(
        IWorldEntity* entity,
        base::CustomLogicTemplate* t,
        std::string spinnerId, std::string spinnerSymbolId
    );
    prot void createBMain() override;

    priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

    pub ~CustomLogicModule_MatchFindPlatformA() override;
};
};
