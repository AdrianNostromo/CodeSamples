#pragma once

#include <base/gh.h>

class IWorldEntity;

namespace base {
class WrappedWorldEntity {pub dCtor(WrappedWorldEntity);
	// This is used to get a pointer to this entity that "knows" if the entity exists or was deleted or stuff.
	pub IWorldEntity* entity;

	pub explicit WrappedWorldEntity(IWorldEntity* entity);

	pub virtual ~WrappedWorldEntity();
};
};
