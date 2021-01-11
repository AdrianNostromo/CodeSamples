#pragma once

#include <base/gh.h>
#include "IAction.h"
#include <base/object/Object.h>
#include <string>
#include <base/list/ListDL.h>

namespace worldGame3D {
class A_Base : public base::Object, virtual public IAction {priv typedef base::Object super;pub dCtor(A_Base);
	prot static const bool IS_DEBUG_ACTIONS;

	pub int type;
	pub std::string name;

	pub bool isSyncedToServer;

	// This is init in constructors to avoid constuctor params spamming.
	prot bool canHaveNoListeners = false;

	// 0;// Removed after 1 tick. Time deltas will be set to 0.
	// <0;// Infinite time persistence. Relies on action logic to end the persistence.
	// >0;// Fixed time persistence. The last frame will make sure totalDeltaS is the target persistence time.
	// The timer "MS" version is the actual one. The float one is the "ms" on converted to "s".
	pub int persistenceDurationMS;
	priv bool isActionLogicFinished = false;

	priv int groupsBitMap;
	
	prot ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* actionsListEntry = nullptr;

	// Data. Persistent.
	// This is not incremented on pre-injection requests returns because it will be called again on the same tick.
	pub int ticksCounter = 0;

	pub int totalDeltaMS = 0;
	pub float totalDeltaS = 0.0f;

	// Data. Tick.
	pub int deltaMS = 0;
	pub float deltaS = 0.0f;

	// This is used to avoid dynamic cast.
	// It contains the subclass void*;
	prot void* rawSubclassPointer = nullptr;

    pub explicit A_Base(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int groupsBitMap);

	pub int getType() final;
	pub std::string getName() final;

	pub bool getIsSyncedToServer() final;

	pub bool getCanHaveNoListeners() final;

	pub int getPersistenceDurationMS() final;
	pub bool getIsFinished() final;
	
	pub int getGroupsBitMap() final;

	pub ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* getActionsListEntry() final;
	pub void setActionsListEntry(ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* actionsListEntry) final;

	pub float getTotalDeltaS() final;
	pub int getTotalDeltaMS() final;
	pub void setTotalDeltaTimes(int totalDeltaMS) final;

	pub void incrementTicksCounter() final;

	pub void setData(int deltaMS) final;
	pub void clearData() final;

	// This is a general finis flag setter, it may be called by any action.
	pub void persistentActionFinished();

	pub void* getRawSubclassPointer(int validationType) final;

	pub void removeFromActionsList(int actionsStatus) final;
	prot virtual void onRemoveFromActionsList_pre(int actionsStatus);

    pub ~A_Base() override;
};
};
