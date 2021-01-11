#pragma once

#include <base/gh.h>
#include "EA_Base.h"

namespace worldGame3D {
class EntityAction : public EA_Base {priv typedef EA_Base super; pub dCtor(EntityAction);
	//asdA36;// use more complex priorities.
	pub class Priorities {
		// This is used for non-intrussive semi-optional actions (mouse_turn).
		pub static const int Low;
		// Normal actions (move, stackable_pickup).
		pub static const int Normal;
		pub static const int NormalPlus1;
		pub static const int High;
		// This is used for stops generally.
		pub static const int UltaHight;
	};

	pub explicit EntityAction(
		int type, std::string name,
		bool isSyncedToServer, 
		int persistenceDurationMS,
		int localEntityId, int syncedEntityId,
		int priority, 
		int groupsBitMap, 
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask,
		int overridesGroupsBitMask);

    pub ~EntityAction() override;
};
};
