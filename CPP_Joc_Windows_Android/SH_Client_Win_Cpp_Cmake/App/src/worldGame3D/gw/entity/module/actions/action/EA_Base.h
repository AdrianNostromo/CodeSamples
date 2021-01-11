#pragma once

#include <base/gh.h>
#include <worldGame3D/actionsSystem/action/Action.h>
#include "IEntityAction.h"

namespace worldGame3D {
class EA_Base : public Action, virtual public IEntityAction {priv typedef Action super;pub dCtor(EA_Base);
	// This is used to generate unique,single groups that can be combined into groupsBitMap and stoppedGroupsBitMask.
	/// These groups are located in Interfaces of modules probably.
	pub static int GetNextAvailableGroupSingleBitMap(); 
	
	prot static int NewTypeIndex();

	pub int localEntityId;
	pub int syncedEntityId;

	pub explicit EA_Base(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int localEntityId, int syncedEntityId,
		int priority, 
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask,
		int overridesGroupsBitMask);

    pub ~EA_Base() override;
};
};
