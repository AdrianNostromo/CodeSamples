#pragma once

#include <base/gh.h>
#include "IComponent.h"

// Note. Use a single view category because views contains sub components 
class IComponent_UIView : public virtual IComponent {priv typedef IComponent super;
	pub static int COMPONENT_CATEGORY;
    pub int getComponentCategory() final;

    pub ~IComponent_UIView() override = default;
};
