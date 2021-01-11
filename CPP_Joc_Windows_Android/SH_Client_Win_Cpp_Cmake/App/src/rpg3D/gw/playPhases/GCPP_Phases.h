#pragma once

#include <base/gh.h>
#include "GCPP_RegisteredEntities.h"
#include <memory>
#include <base/list/ListDL.h>
#include "util/IPlayPhaseInteractor.h"
#include <base/memory/SharedPointer.h>
#include <base/MM.h>

namespace base {
    class GWEvent;
    class WrappedWorldEntity;
};
namespace rpg3D {
    class IGPGrid2DContent;
};

namespace rpg3D {
namespace playPhases {
class GCPP_Phases : public GCPP_RegisteredEntities {priv typedef GCPP_RegisteredEntities super;pub dCtor(GCPP_Phases);
    priv static const bool IS_DEBUG_LOCAL;

    priv bool isPlayPhasesEnabled = false;
    priv int activePhaseIndex = -1;
    priv TurnPhaseConfig* activePhaseConfig = nullptr;

    // This is used to avoid phases beying instantly ended by user button spam from the previous phase.
    /// This does a ~0.2 delay until a phase can be ended by user input.
    priv float userPhaseEndInputSpamSupressorS = 0.0f;
    priv bool isUserPhaseEndRequest = false;

    // This is set to nullptr if the focus entity gets unregistered. The nextTurnFocusEntity is used in that case.
    priv ListDL<sp<RegisteredEntry>>::Entry* turnFocusEntity = nullptr;
    priv ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity = nullptr;
    priv int turnTeamIndexInTurnBasedTeamsList = 0;
    priv int turnTeamId = 0;
    priv int turnIndex = 0;
    
    priv rpg3D::IGPGrid2DContent* gpGrid2DContent = nullptr;

    pub explicit GCPP_Phases(
        base::IGameWorld* gw,
        sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList);
    prot void createBMain() override;

    pub int getTurnIndex() final;
    priv void incrementTurnIndex();

    priv static ListDL<sp<RegisteredEntry>>::Entry* GetNextTurnFocusEntity(ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity, int turnTeamId, int turnIndex);

    priv void onGWEvent(IEventListener& eventListener, base::GWEvent& event);

    priv void setActivePhaseIndex(int newPhaseIndex);

    priv int tickActivePhase(float deltaS);
    priv void onActivePhaseDeactivationPre();
    priv void onActivePhaseActivated();

    pub void setIsPlayPhasesEnabled(bool isPlayPhasesEnabled) final;
    
    pub void onUserInput_endCurrentPlayPhase() final;

    prot void disposeMain() override;
    pub ~GCPP_Phases() override;
};
};
};
