#pragma once

#include <base/gh.h>
#include "T_Base.h"

namespace rpg3D {
class T_Events : public T_Base {priv typedef T_Base super;pub dCtor(T_Events);
	prot base::TypedEventDispatcher1D<rpg3D::ToolEvent> events{};

	pub explicit T_Events(ToolConfigBase* config);

	pub base::TypedEventDispatcher1D<rpg3D::ToolEvent>& getEvents() final;

    pub ~T_Events() override;
};
};
