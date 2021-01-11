#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <memory>
#include "IConstraint3D.h"

class btTypedConstraint;
class btDiscreteDynamicsWorld;
namespace rpg3D {
	class CfgConstraint3D;
	class IHandlerContraint3D;
	class IPhysics3DModule;

class Constraint3D final/*Note1001. calls_the_reserved_dispose*/ : public base::Object, virtual public IConstraint3D {priv typedef base::Object super;pub dCtor(Constraint3D);
	pub IHandlerContraint3D* handler;

	pub std::shared_ptr<CfgConstraint3D> cfg;

	pub btTypedConstraint* constraint_managed;

	pub IPhysics3DModule* otherTarget;

	priv btDiscreteDynamicsWorld* pWorld = nullptr;

    pub explicit Constraint3D(
		IHandlerContraint3D* handler, std::shared_ptr<CfgConstraint3D> cfg, btTypedConstraint* constraint_managed,
		IPhysics3DModule* otherTarget);

	pub void addToPWorld(btDiscreteDynamicsWorld* pWorld);
	pub void removeFromPWorld();

	pub void removeConstraint() final;

	prot void disposeMain() override;
	pub ~Constraint3D() override;
};
};
