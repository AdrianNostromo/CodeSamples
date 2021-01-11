#pragma once

#include <base/gh.h>
#include "ABase.h"
#include <mutex>
#include <memory>
#include <base/list/ArrayList.h>

namespace base {
class APreRendering : public ABase {priv typedef ABase super;pub dCtor(APreRendering);
    priv ArrayList<std::shared_ptr<AnimatableControl>> animatablesList{};

    priv std::mutex* mtx;

    pub explicit APreRendering();

    pub std::shared_ptr<IAnimatableControl> addPreRenderingAnimatable(IAnimatable* target) override;
    pub void removePreRenderingAnimatable(std::shared_ptr<IAnimatableControl>& target) override;

    pub void loopTickPreRendering(float deltaS, int deltaMS);

    pub ~APreRendering() override;
};
};
