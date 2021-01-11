#pragma once

#include <base/gh.h>
#include "P3DM_Positional.h"
#include <base/list/ArrayList.h>
#include <memory>

namespace rpg3D {
	class PhysicsEntityCallbackFilter;
	class PhysicsActor;
};

namespace rpg3D {
class P3DM_Contacts : public P3DM_Positional {priv typedef P3DM_Positional super;pub dCtor(P3DM_Contacts);
	priv rpg3D::PhysicsActor* physicsActor = nullptr;

    pub explicit P3DM_Contacts(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);
	prot void createMain() override;

	pub btRigidBody* getPBodyVisualsAnchor() final;

	pub IWorldEntity* getEntity() override;

	pub IPhysicsEntityCallbackFilter* addEntityContactFilter(std::function<void(
		IPhysicsEntityCallbackFilter * filter, IWorldEntity * otherEntity, bool isContact)> cb_onContact) final;
	pub void removeEntityContactFilter(IPhysicsEntityCallbackFilter* localFilter) final;

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	prot void disposePre() override;
	prot void disposeMain() override;
	pub ~P3DM_Contacts() override;
};
};
