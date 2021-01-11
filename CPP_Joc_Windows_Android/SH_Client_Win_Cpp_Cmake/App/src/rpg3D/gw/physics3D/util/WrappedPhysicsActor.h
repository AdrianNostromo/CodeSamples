#pragma once

#include <base/gh.h>


namespace rpg3D {
	class IPhysicsActor;

class WrappedPhysicsActor {pub dCtor(WrappedPhysicsActor);
	// This is used to get a pointer to this physicsActor that "knows" if the physicsActor exists or was deleted or stuff.
	pub IPhysicsActor* physicsActor;

	pub explicit WrappedPhysicsActor(IPhysicsActor* physicsActor);

	pub virtual ~WrappedPhysicsActor();
};
};
