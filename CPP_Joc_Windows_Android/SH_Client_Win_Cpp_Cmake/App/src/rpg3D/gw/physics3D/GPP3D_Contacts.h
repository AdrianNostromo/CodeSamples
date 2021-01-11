#pragma once

#include <base/gh.h>
#include "GPP3D_Base.h"
#include "base/list/ListDL.h"
#include <base/MM.h>

namespace base {
	class WrappedWorldEntity;
};
namespace rpg3D {
	class IPhysicsActor;

class GPP3D_Contacts : public GPP3D_Base {priv typedef GPP3D_Base super;pub dCtor(GPP3D_Contacts);
	priv class EntitiesContact {
		pub IPhysicsActor* physicsActorA;

		pub IPhysicsActor* physicsActorB;
	};

	priv ListDL<EntitiesContact> filteredContactsList{LOC};

	pub explicit GPP3D_Contacts(base::IGameWorld* gw);
	
	prot void processContactChange(IPhysicsActor* physicsActorA, IPhysicsActor* physicsActorB, bool isContact);

	pub ~GPP3D_Contacts() override;
};
};
