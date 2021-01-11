#pragma once

#include <base/animator/IAnimator.h>
#include "Sprite2D.h"
#include <graphics/visual2d/util/AnimatedList.h>

class MovieClip2D : public Sprite2D, virtual public base::IAnimator::IAnimatable {
private: typedef Sprite2D super;
public:
    bool pauseOnLoopRestart = false;

private:
    base::IAnimator* animator;

    std::shared_ptr<base::AnimatedList<TextureRegion*>> framesAnimatedList;

    std::shared_ptr<base::IAnimator::IAnimatableControl> animatableControl = nullptr;


    float stateTime = 0.0f;
    bool isPlaying = true;

    int keyFrameIndexOverride = -1;

public:
    MovieClip2D(MovieClip2D const&) = delete;
    MovieClip2D(MovieClip2D &&) = default;
    MovieClip2D& operator=(MovieClip2D const&) = delete;
    MovieClip2D& operator=(MovieClip2D &&) = default;

    explicit MovieClip2D(base::IAnimator* animator, std::shared_ptr<base::AnimatedList<TextureRegion*>> framesAnimatedList);

    void play(bool restart);
    void pause();
    void stop();
    void setKeyFrameIndexOverride(int keyFrameIndexOverride);

    void onRootChanged() override;

    void onGlobal_isVisibleChanged() override;

    ~MovieClip2D() override;
protected:
    void disposeMain() override;
private:
    void updateAnimatorAsNeeded();
    void onAnimationTick(float deltaS, int deltaMS) override;

};
