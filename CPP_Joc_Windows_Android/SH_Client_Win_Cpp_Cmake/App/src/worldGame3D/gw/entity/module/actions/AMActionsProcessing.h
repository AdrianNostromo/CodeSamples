#pragma once

#include <base/gh.h>
#include "AMProcessorsList.h"

namespace worldGame3D {
class AMActionsProcessing : public AMProcessorsList {priv typedef AMProcessorsList super; pub dCtor(AMActionsProcessing);
	priv static const bool IS_DEBUG_ACTIONS;

	pub explicit AMActionsProcessing(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

	priv void onGWEvent_gameLoop_actionsProcessingSimulation(IEventListener& eventListener, base::GWEvent& event);

	priv void dispatchReplacedActionsAndClear(
		ArrayList<std::shared_ptr<worldGame3D::IAction>>* replacedActionsList);

    pub ~AMActionsProcessing() override;

};
};
