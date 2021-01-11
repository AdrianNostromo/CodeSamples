#include <base/math/util/BoolUtil.h>
#include "AMActionsSync.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace worldGame3D;

AMActionsSync::AMActionsSync(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(entity, t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_actionsSending->type,
		std::bind(&AMActionsSync::onGWEvent_gameLoop_actionsSending, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

//asd2;// Sync problems exist with this system.
/// In town a, the player painter panel has to stop the player and move,rotate him to a specific position.
/// Those actions are not synced, need to refactor this to also sync those actions.
/// Maybe get the actions directly from the player entities using a listener.
/// Also need to sync setPos, setRot local actions (those are also used by the entity painter).

void AMActionsSync::onGWEvent_gameLoop_actionsSending(IEventListener& eventListener, base::GWEvent& event) {
	// Don't send the update if there are no syncable actions.
	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* listEntry;
	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* nextEnumeratingEntry = actionsList.getFirst();
	while ((listEntry = nextEnumeratingEntry) != nullptr) {
		nextEnumeratingEntry = nextEnumeratingEntry->next;

		std::shared_ptr<worldGame3D::IAction> action = listEntry->data;
		(void)action;

		/*asd000;// refactor to new system when used.
		if(isAnySyncableAction()) {
			if (scenarioConfig.serverConnectInfo != null) {
				// Pack the actions in a buffer and send it.
				int bytesLen = 0;

				// Actions count;
				bytesLen += 4;

				try {
					bytesLen += computeActionsBytesLen();
				} catch (Exception x) {
					handler.onLogicError();
					return;
				}

				BasicBuffer actionsBuffer = new BasicBuffer(ByteBuffer.allocate(bytesLen));

				int syncedActionsCount = countSyncedActionFromList(actionsList);
				try {
					actionsBuffer.writeInt(syncedActionsCount);
				} catch (Exception x) {
					handler.onLogicError();
					return;
				}

				try {
					writeActionsToBuffer(actionsBuffer);
				} catch (Exception x) {
					handler.onLogicError();
					return;
				}

				try {
					server.sendMsg_CS_C_HandlePlayerActions(actionsBuffer);
				} catch (Exception x) {
					handler.onNetError();
					return;
				}
			}
		}*/
	}
}

int AMActionsSync::countSyncedActionFromList(ArrayList<worldGame3D::IAction*>& actionsList) {
	int ct = 0;

	for(int i=0;i<actionsList.size();i++) {
		worldGame3D::IAction& action = *actionsList.getDirect(i);

		if(action.getIsSyncedToServer()) {
			ct++;
		}
	}

	return ct;
}

AMActionsSync::~AMActionsSync() {
	//void
}
