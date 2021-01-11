#pragma once

#include <memory>

namespace base {
class IAnimator {
public:
    class IAnimatable {
    public:
        virtual void onAnimationTick(float deltaS, int deltaMS) = 0;

        virtual ~IAnimatable() = default;
    };
    class IAnimatableControl {
    public:
        virtual void removeListener() = 0;

        virtual ~IAnimatableControl() = default;
    };
public:
    virtual std::shared_ptr<IAnimatableControl> addPreRenderingAnimatable(IAnimatable* target) = 0;
    virtual void removePreRenderingAnimatable(std::shared_ptr<IAnimatableControl>& target) = 0;

    virtual ~IAnimator() = default;
};
};
