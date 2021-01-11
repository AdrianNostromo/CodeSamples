#pragma once

#include <base/gh.h>
#include "IComponent.h"

// NOTE. This is a example implementation.

class IComponent_ServerComponent : public virtual IComponent {priv typedef IComponent super;
	pub static int COMPONENT_CATEGORY;
    pub int getComponentCategory() final;

    pub ~IComponent_ServerComponent() override = default;
};
