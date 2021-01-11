#pragma once

#include <base/gh.h>
#include "AMActionsProcessing.h"
#include <base/list/ArrayList.h>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

namespace worldGame3D {
class AMActionsSync : public AMActionsProcessing {priv typedef AMActionsProcessing super; pub dCtor(AMActionsSync);
	pub explicit AMActionsSync(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

	priv void onGWEvent_gameLoop_actionsSending(IEventListener& eventListener, base::GWEvent& event);

	priv static int countSyncedActionFromList(ArrayList<worldGame3D::IAction*>& actionsList);

    pub ~AMActionsSync() override;
};
};
