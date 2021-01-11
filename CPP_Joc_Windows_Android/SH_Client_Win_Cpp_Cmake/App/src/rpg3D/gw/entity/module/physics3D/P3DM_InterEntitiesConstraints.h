#pragma once

#include <base/gh.h>
#include "P3DM_Body.h"
#include "constraint3D/IHandlerContraint3D.h"

namespace rpg3D {
class P3DM_InterEntitiesConstraints : public P3DM_Body, virtual public IHandlerContraint3D {priv typedef P3DM_Body super;pub dCtor(P3DM_InterEntitiesConstraints);
	priv ArrayList<std::shared_ptr<Constraint3D>> constraintsList{};
	priv ArrayList<IConstraint3D*> remoteConstraintsList{};

    pub explicit P3DM_InterEntitiesConstraints(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);

	prot void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) override;
	prot void disconnectFromGamePartsPre() override;

	pub IConstraint3D* createConstraint(std::shared_ptr<CfgConstraint3D> config, IPhysics3DModule* target) final;
	pub void pushRemoteConstraint(IConstraint3D* remoteConstraint) final;
	pub void removeRemoteConstraint(IConstraint3D* remoteConstraint) final;

	pub void removeConstraint(Constraint3D* constraint) final;

	priv std::shared_ptr<Constraint3D> buildConstraint(std::shared_ptr<CfgConstraint3D> config, IPhysics3DModule* target);

	prot void disposeMain() override;
	pub ~P3DM_InterEntitiesConstraints() override;
};
};
