#include "MovieClip2D.h"
#include <base/math/util/BoolUtil.h>

MovieClip2D::MovieClip2D(base::IAnimator* animator, std::shared_ptr<base::AnimatedList<TextureRegion*>> framesAnimatedList)
    : super(framesAnimatedList->getKeyFrame(0.0f)),
      animator(animator), framesAnimatedList(framesAnimatedList)
{
    //void
}

void MovieClip2D::updateAnimatorAsNeeded() {
    if(!isPlaying || getRoot() == nullptr || !getGlobal_isVisible()) {
        if(animatableControl != nullptr) {
            animator->removePreRenderingAnimatable(animatableControl);

            animatableControl = nullptr;
        }
    }else {
        if(animatableControl == nullptr) {
            animatableControl = animator->addPreRenderingAnimatable(this);
        }
    }
}

void MovieClip2D::onAnimationTick(float deltaS, int deltaMS) {
    if(isPlaying) {
        if(pauseOnLoopRestart && framesAnimatedList->getKeyFrameIndex(stateTime + deltaS) < framesAnimatedList->getKeyFrameIndex(stateTime)) {
            pauseOnLoopRestart = false;

            isPlaying = false;
            stateTime = 0;
        }

        stateTime += deltaS;
    }

    TextureRegion* currentFrameTexture;
    if(keyFrameIndexOverride >= 0) {
        currentFrameTexture = framesAnimatedList->getKeyFrames()->getDirect(keyFrameIndexOverride);
    }else {
        currentFrameTexture = framesAnimatedList->getKeyFrame(stateTime);
    }

    if(getTextureRegion() != currentFrameTexture) {
        setTextureRegion(currentFrameTexture);
    }
}

void MovieClip2D::play(bool restart) {
    isPlaying = true;
    if(restart) {
        stateTime = 0.0f;
    }

    pauseOnLoopRestart = false;

    updateAnimatorAsNeeded();
}

void MovieClip2D::pause() {
    isPlaying = false;

    pauseOnLoopRestart = false;

    updateAnimatorAsNeeded();
}

void MovieClip2D::stop() {
    isPlaying = false;
    stateTime = 0.0f;

    pauseOnLoopRestart = false;

    updateAnimatorAsNeeded();
}

void MovieClip2D::setKeyFrameIndexOverride(int keyFrameIndexOverride) {
    this->keyFrameIndexOverride = keyFrameIndexOverride;
}

void MovieClip2D::onRootChanged() {
    super::onRootChanged();

    updateAnimatorAsNeeded();
}

void MovieClip2D::onGlobal_isVisibleChanged() {
    super::onGlobal_isVisibleChanged();

    updateAnimatorAsNeeded();
}

void MovieClip2D::disposeMain() {
    if(animatableControl != nullptr) {
        animator->removePreRenderingAnimatable(animatableControl);

        animatableControl = nullptr;
    }

    super::disposeMain();
}

MovieClip2D::~MovieClip2D() {
    //void
}
