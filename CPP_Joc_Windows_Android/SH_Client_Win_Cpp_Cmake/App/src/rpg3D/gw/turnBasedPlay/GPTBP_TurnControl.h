#pragma once

#include <base/gh.h>
#include "GPTBP_Base.h"
#include <memory>
#include <base/list/ListDL.h>
#include "util/ITurnBasedInteractor.h"
#include <base/memory/SharedPointer.h>
#include <base/MM.h>

namespace base {
    class GWEvent;
    class WrappedWorldEntity;
};

namespace rpg3D {
class GPTBP_TurnControl : public GPTBP_Base {priv typedef GPTBP_Base super;pub dCtor(GPTBP_TurnControl);
    priv class RegisteredEntry : public virtual ITurnBasedInteractor {pub dCtor(RegisteredEntry);
        pub GPTBP_TurnControl* handler;
        pub ListDL<sp<RegisteredEntry>>::Entry* inHandler_listEntry = nullptr;

        pub ITurnBasedEntity* turnBasedEntity;

        // Use a setter so that the entity gets notified of the change.
        priv ActionType currentActionType = None;
        pub bool isCurrentActionFinshed = false;

        // This is used so that entities created on the active turn and active turnTeamIndexInTurnBasedTeamsList will only activate on the next turn.
        /// eg. A unit used an action to spawn another ally unit. The spawned unit will only act on the next turn.
        // The entity will activate on the current turn is it got registered while another turnTeamIndexInTurnBasedTeamsList was active.
        /// eg. A enemy destroyed a unit and that unit had a buff to split into 2 entities on death. Those 2 parts will act on the current turn if they are on a team that hasn't already acted this turn. If the 2 parts team already acted this turn, the 2 entities will start acting on the next turn.
        pub int startsActingAfterTurnIndex = -1;

        pub explicit RegisteredEntry(
            GPTBP_TurnControl* handler,
            ITurnBasedEntity* turnBasedEntity);

        pub void unregister() final;

        pub ActionType getCurrentActionType() final;
        pub void setCurrentActionType(ActionType currentActionType);

        pub bool getIsCurrentActionFinshed() final;

        pub void currentActionFinished(ActionType finishedActionType) final;

    };
    priv static const bool IS_DEBUG_LOCAL;

    // Entries are removed on next turn process, they are not removed on entity unregistering.
    priv ListDL<sp<RegisteredEntry>> registeredTurnBasedEntities{LOC};

    priv bool isTurnBasedPlayEnabled = false;
    priv bool isTurnBasedPlayTickChanged = false;
    
    // This is set to nullptr if the focus entity gets unregistered. The nextTurnFocusEntity is used in that case.
    priv ListDL<sp<RegisteredEntry>>::Entry* turnFocusEntity = nullptr;
    priv ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity = nullptr;
    priv int turnTeamIndexInTurnBasedTeamsList = 0;
    priv int turnTeamId = 0;
    priv int turnIndex = 0;
    
    pub explicit GPTBP_TurnControl(
        base::IGameWorld* gw,
        TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList);

    pub int getTurnIndex() final;
    priv void incrementTurnIndex();

    priv static ListDL<sp<RegisteredEntry>>::Entry* GetNextTurnFocusEntity(ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity, int turnTeamId, int turnIndex);

    pub ITurnBasedInteractor* registerTurnBasedEntity(ITurnBasedEntity* turnBasedEntity) final;
    pub void unregisterTurnBasedEntity(RegisteredEntry* turnBasedInteractor);

    priv void onGWEvent(IEventListener& eventListener, base::GWEvent& event);

    // Only 1 is called from these.
    priv void tickTurnState_SeparateMoveAbilityTurns(float deltaS);
    priv void tickTurnState_SingleActionTurn(float deltaS);

    pub void setIsTurnBasedPlayEnabled(bool isTurnBasedPlayEnabled) final;

    prot void disposeMain() override;
    pub ~GPTBP_TurnControl() override;
};
};
