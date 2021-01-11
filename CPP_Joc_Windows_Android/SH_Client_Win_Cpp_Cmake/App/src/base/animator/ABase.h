#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IAnimator.h"
#include <base/component/IComponent_AppComponent.h>

namespace base {
class ABase : public base::ManagedObject, virtual public IAnimator, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(ABase);
    prot class AnimatableControl : virtual public IAnimatableControl {pub dCtor(AnimatableControl);
        pub IAnimatable* animatable;

        pub explicit AnimatableControl(IAnimatable* animatable);

        pub void removeListener() override;

        pub ~AnimatableControl() override;
    };

    pub explicit ABase();

    pub ~ABase() override;
};
};
