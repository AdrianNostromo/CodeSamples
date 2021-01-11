#include "APreRendering.h"
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

using namespace base;

APreRendering::APreRendering()
        : super()
{
    mtx = new std::mutex();
}

std::shared_ptr<APreRendering::IAnimatableControl> APreRendering::addPreRenderingAnimatable(IAnimatable* target) {
    mtx->lock();

    for(int i=0;i<animatablesList.size();i++) {
        std::shared_ptr<AnimatableControl> entry = animatablesList.getDirect(i);

        if(entry->animatable == target) {
            mtx->unlock();

            throw LogicException(LOC);
        }
    }

    std::shared_ptr<AnimatableControl> aControl = std::make_shared<AnimatableControl>(target);
    animatablesList.appendDirect(aControl);

    mtx->unlock();

    return std::static_pointer_cast<APreRendering::IAnimatableControl>(aControl);
}

void APreRendering::removePreRenderingAnimatable(std::shared_ptr<IAnimatableControl>& target) {
    target->removeListener();
}

void APreRendering::loopTickPreRendering(float deltaS, int deltaMS) {
    mtx->lock();

    for(int i=0;i<animatablesList.size();i++) {
        std::shared_ptr<AnimatableControl> entry = animatablesList.getDirect(i);

        if(entry->animatable == nullptr){
            //Remove the entry.
            if(animatablesList.removeAndGetDirect(i) != entry) {
                throw LogicException(LOC);
            }
            i--;
        }else {
            entry->animatable->onAnimationTick(deltaS, deltaMS);
        }
    }

    mtx->unlock();
}

APreRendering::~APreRendering() {
    if(mtx != nullptr) {
        delete mtx;
        mtx = nullptr;
    }
}
