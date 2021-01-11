#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

class IWorldEntity;
namespace base {
	class EntityEvent;
};

namespace rpg3D {
class IFlyingModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub static base::EntityEvent* EVENT_FlyingFinished_TargetLost;
	pub static base::EntityEvent* EVENT_FlyingFinished_DestinationReached;

	pub virtual void addAction_ActionDirectionFlight(Vector3& velocityDir) = 0;
	pub virtual void addAction_ActionApplyImpulse(
		Vector3& dir, 
		float physicsImpulse) = 0;
	pub virtual void addAction_ActionDestinationFlight(
		IWorldEntity* targetEntity, Vector3& targetPosOrGlobalOffset,
		bool flyToLastKnownTargetPosOnTargetLoss, bool stopFlyingAtDestination) = 0;

    pub ~IFlyingModule() override = default;
};
};
