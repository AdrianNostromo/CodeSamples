#pragma once

#include <base/gh.h>

namespace rpg3D {
namespace playPhases {
class IPlayPhaseInteractor {
    pub enum ActionType { None, AnyAction, Movement, Ability };

    pub virtual void unregister() = 0;

    pub virtual ActionType getCurrentActionType() = 0;
    
    pub virtual bool getIsCurrentActionFinshed() = 0;

    // This can have a None value if the action was skipped.
    pub virtual void currentActionFinished(ActionType finishedActionType) = 0;

    pub virtual ~IPlayPhaseInteractor() = default;
};
};
};
