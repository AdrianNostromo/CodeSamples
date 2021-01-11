#include "ABase.h"

using namespace base;

ABase::AnimatableControl::AnimatableControl(IAnimatable* animatable)
    : animatable(animatable)
{
    //void
}

void ABase::AnimatableControl::removeListener() {
    // This causes this AnimatableControl to be removed before the next animatable->tick() call.
    animatable = nullptr;
}

ABase::AnimatableControl::~AnimatableControl() {
    //void
}

ABase::ABase()
    : super()
{
    //void
}

ABase::~ABase() {
    //void
}
