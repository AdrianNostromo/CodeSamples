#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/component/IComponent_GameWorldComponent.h>

namespace base {
	class IGameWorld;
};

class IGamePart : public virtual IComponent_GameWorldComponent {
	pub virtual base::IGameWorld* getGW() = 0;

    pub virtual ~IGamePart() = default;
};
