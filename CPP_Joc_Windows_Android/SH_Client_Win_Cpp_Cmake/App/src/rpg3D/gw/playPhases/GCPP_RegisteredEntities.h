#pragma once

#include <base/gh.h>
#include "GCPP_Base.h"
#include <memory>
#include <base/list/ListDL.h>
#include "util/IPlayPhaseInteractor.h"
#include <base/memory/SharedPointer.h>
#include <base/MM.h>

namespace rpg3D {
namespace playPhases {
class GCPP_RegisteredEntities : public GCPP_Base {priv typedef GCPP_Base super;pub dCtor(GCPP_RegisteredEntities);
    prot class RegisteredEntry : public virtual IPlayPhaseInteractor {pub dCtor(RegisteredEntry);
        pub GCPP_RegisteredEntities* handler;
        pub ListDL<sp<RegisteredEntry>>::Entry* inHandler_listEntry = nullptr;

        pub IPhaseEntity* turnBasedEntity;

        // Use a setter so that the entity gets notified of the change.
        priv ActionType currentActionType = None;
        pub bool isCurrentActionFinshed = false;

        // This is used so that entities created on the active turn and active turnTeamIndexInTurnBasedTeamsList will only activate on the next turn.
        /// eg. A unit used an action to spawn another ally unit. The spawned unit will only act on the next turn.
        // The entity will activate on the current turn is it got registered while another turnTeamIndexInTurnBasedTeamsList was active.
        /// eg. A enemy destroyed a unit and that unit had a buff to split into 2 entities on death. Those 2 parts will act on the current turn if they are on a team that hasn't already acted this turn. If the 2 parts team already acted this turn, the 2 entities will start acting on the next turn.
        pub int startsActingAfterTurnIndex = -1;

        pub explicit RegisteredEntry(
            GCPP_RegisteredEntities* handler,
            IPhaseEntity* turnBasedEntity);

        pub void unregister() final;

        pub ActionType getCurrentActionType() final;
        pub void setCurrentActionType(ActionType currentActionType);

        pub bool getIsCurrentActionFinshed() final;

        pub void currentActionFinished(ActionType finishedActionType) final;

    };

    // Entries are removed on next turn process, they are not removed on entity unregistering.
    prot ListDL<sp<RegisteredEntry>> registeredTurnBasedEntities{ LOC };

    pub explicit GCPP_RegisteredEntities(
        base::IGameWorld* gw,
        sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList);

    asd;// remove layer;

    pub IPlayPhaseInteractor* registerTurnBasedEntity(IPhaseEntity* turnBasedEntity) final;
    pub void unregisterTurnBasedEntity(RegisteredEntry* turnBasedInteractor);

    pub ~GCPP_RegisteredEntities() override;
};
};
};
