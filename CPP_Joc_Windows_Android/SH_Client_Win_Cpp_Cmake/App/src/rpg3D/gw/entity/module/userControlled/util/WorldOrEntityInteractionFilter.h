#pragma once

#include <base/gh.h>
#include "IWorldOrEntityInteractionFilter.h"
#include <rpg3D/gw/entity/filter/EntityFilter.h>

namespace rpg3D {
class WorldOrEntityInteractionFilter : public EntityFilter, virtual public IWorldOrEntityInteractionFilter {priv typedef EntityFilter super;pub dCtor(WorldOrEntityInteractionFilter);
	pub int type;

	pub explicit WorldOrEntityInteractionFilter(const int type);

	pub int getType() override;

	pub ~WorldOrEntityInteractionFilter() override;
};
};
