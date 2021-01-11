#include "IAnimatedComposedStiledVisual.h"

IAnimatedComposedStiledVisual::CbAnimationListener::CbAnimationListener() {
    //void
}

void IAnimatedComposedStiledVisual::CbAnimationListener::animationFinished(IGenericDrawable* target) {
    if(cb_onAnimationFinished) {
        cb_onAnimationFinished(target);
    }
}

IAnimatedComposedStiledVisual::CbAnimationListener::~CbAnimationListener() {
    //void
}
