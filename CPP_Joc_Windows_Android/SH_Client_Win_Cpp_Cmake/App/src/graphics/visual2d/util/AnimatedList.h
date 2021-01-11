#pragma once

#include <memory>
#include <graphics/visual/TextureRegion.h>
#include <base/list/ArrayList.h>
#include <base/math/Math.h>
#include "PlayMode.h"

namespace base {
template <typename T>
class AnimatedList {
private:
    /** Length must not be modified without updating {@link #animationDuration}. See {@link #setKeyFrames(T[])}. */
    std::shared_ptr<ArrayList<TextureRegion*>> keyFrames;
    float frameDuration;
    float animationDuration = 0;
    int lastFrameNumber = 0;
    float lastStateTime = 0.0f;

    PlayMode playMode = PlayMode::NORMAL;

public:
    AnimatedList(AnimatedList const&) = delete;
    AnimatedList(AnimatedList &&) = default;
    AnimatedList& operator=(AnimatedList const&) = delete;
    AnimatedList& operator=(AnimatedList &&) = default;
    explicit AnimatedList(
        float frameDuration, std::shared_ptr<ArrayList<TextureRegion*>> keyFrames);
    explicit AnimatedList(
        float frameDuration, std::shared_ptr<ArrayList<TextureRegion*>> keyFrames,
        PlayMode playMode);

    T getKeyFrame (float stateTime, bool looping);
    T getKeyFrame (float stateTime);

    int getKeyFrameIndex(float stateTime);

    std::shared_ptr<ArrayList<TextureRegion*>> getKeyFrames();
    void setKeyFrames (std::shared_ptr<ArrayList<TextureRegion*>> keyFrames);

    PlayMode getPlayMode();
    void setPlayMode(PlayMode playMode);

    bool isAnimatedListFinished(float stateTime);

    float getFrameDuration();
    void setFrameDuration (float frameDuration);

    float getAnimatedListDuration();

    virtual ~AnimatedList();
};

/** Constructor, storing the frame duration and key frames.
 *
 * @param frameDuration the time between frames in seconds.
 * @param keyFrames the objects representing the frames. If this Array is type-aware, {@link #getKeyFrames()} can return the
 *           correct type of array. Otherwise, it returns an Object[]. */
template <typename T>
AnimatedList<T>::AnimatedList(
    float frameDuration, std::shared_ptr<ArrayList<TextureRegion*>> keyFrames)
    : frameDuration(frameDuration)
{
    setKeyFrames(keyFrames);
}

/** Constructor, storing the frame duration and key frames.
 *
 * @param frameDuration the time between frames in seconds.
 * @param keyFrames the objects representing the frames. If this Array is type-aware, {@link #getKeyFrames()} can
 * return the correct type of array. Otherwise, it returns an Object[].*/
template <typename T>
AnimatedList<T>::AnimatedList(
    float frameDuration, std::shared_ptr<ArrayList<TextureRegion*>> keyFrames,
    PlayMode playMode)
    : AnimatedList(frameDuration, keyFrames)
{
    setPlayMode(playMode);
}

/** Returns a frame based on the so called state time. This is the amount of seconds an object has spent in the
 * state this AnimatedList instance represents, e.g. running, jumping and so on. The mode specifies whether the animation is
 * looping or not.
 *
 * @param stateTime the time spent in the state represented by this animation.
 * @param looping whether the animation is looping or not.
 * @return the frame of animation for the given state time. */
template <typename T>
T AnimatedList<T>::getKeyFrame(float stateTime, bool looping) {
    // we set the play mode by overriding the previous mode based on looping
    // parameter value
    PlayMode oldPlayMode = playMode;
    if (looping && (playMode == PlayMode::NORMAL || playMode == PlayMode::REVERSED)) {
        if (playMode == PlayMode::NORMAL)
            playMode = PlayMode::LOOP;
        else
            playMode = PlayMode::LOOP_REVERSED;
    } else if (!looping && !(playMode == PlayMode::NORMAL || playMode == PlayMode::REVERSED)) {
        if (playMode == PlayMode::LOOP_REVERSED)
            playMode = PlayMode::REVERSED;
        else
            playMode = PlayMode::LOOP;
    }

    T frame = getKeyFrame(stateTime);
    playMode = oldPlayMode;
    return frame;
}

/** Returns a frame based on the so called state time. This is the amount of seconds an object has spent in the
 * state this AnimatedList instance represents, e.g. running, jumping and so on using the mode specified by
 * {@link #setPlayMode(PlayMode)} method.
 *
 * @param stateTime
 * @return the frame of animation for the given state time. */
template <typename T>
T AnimatedList<T>::getKeyFrame(float stateTime) {
    int frameNumber = getKeyFrameIndex(stateTime);
    return keyFrames->getDirect(frameNumber);
}

/** Returns the current frame number.
 * @param stateTime
 * @return current frame number */
template <typename T>
int AnimatedList<T>::getKeyFrameIndex(float stateTime) {
    if (keyFrames->count() == 1) {
        return 0;
    }

    int frameNumber = (int)(stateTime / frameDuration);
    switch (playMode) {
        case NORMAL: {
            frameNumber = Math::min(keyFrames->count() - 1, frameNumber);

            break;
        }
        case LOOP: {
            frameNumber = frameNumber % keyFrames->count();

            break;
        }
        case LOOP_PINGPONG: {
            frameNumber = frameNumber % ((keyFrames->count() * 2) - 2);
            if (frameNumber >= keyFrames->count()) frameNumber = keyFrames->count() - 2 - (frameNumber - keyFrames->count());

            break;
        }
        case LOOP_RANDOM: {
            int lastFrameNumber = (int) ((lastStateTime) / frameDuration);
            if (lastFrameNumber != frameNumber) {
                frameNumber = Math::Rand::nextInt(keyFrames->count() - 1);
            } else {
                frameNumber = this->lastFrameNumber;
            }

            break;
        }
        case REVERSED: {
            frameNumber = Math::max(keyFrames->count() - frameNumber - 1, 0);

            break;
        }
        case LOOP_REVERSED: {
            frameNumber = frameNumber % keyFrames->count();
            frameNumber = keyFrames->count() - frameNumber - 1;

            break;
        }
    }

    lastFrameNumber = frameNumber;
    lastStateTime = stateTime;

    return frameNumber;
}

/** Returns the keyframes[] array where all the frames of the animation are stored.
 * @return The keyframes[] field. This array is an Object[] if the animation was instantiated with an Array that was not
 *         type-aware. */
template <typename T>
std::shared_ptr<ArrayList<TextureRegion*>> AnimatedList<T>::getKeyFrames() {
    return keyFrames;
}

template <typename T>
void AnimatedList<T>::setKeyFrames(std::shared_ptr<ArrayList<TextureRegion*>> keyFrames) {
    this->keyFrames = keyFrames;
    this->animationDuration = keyFrames->count() * frameDuration;
}

/** Returns the animation play mode. */
template <typename T>
PlayMode AnimatedList<T>::getPlayMode() {
    return playMode;
}

/** Sets the animation play mode.
 *
 * @param playMode The animation {@link PlayMode} to use. */
template <typename T>
void AnimatedList<T>::setPlayMode(PlayMode playMode) {
    this->playMode = playMode;
}

/** Whether the animation would be finished if played without looping (PlayMode#NORMAL), given the state time.
 * @param stateTime
 * @return whether the animation is finished. */
template <typename T>
bool AnimatedList<T>::isAnimatedListFinished(float stateTime) {
    int frameNumber = (int)(stateTime / frameDuration);
    return keyFrames->count() - 1 < frameNumber;
}

/** @return the duration of a frame in seconds */
template <typename T>
float AnimatedList<T>::getFrameDuration() {
    return frameDuration;
}

/** Sets duration a frame will be displayed.
 * @param frameDuration in seconds */
template <typename T>
void AnimatedList<T>::setFrameDuration(float frameDuration) {
    this->frameDuration = frameDuration;
    this->animationDuration = keyFrames->count() * frameDuration;
}

/** @return the duration of the entire animation, number of frames times frame duration, in seconds */
template <typename T>
float AnimatedList<T>::getAnimatedListDuration() {
    return animationDuration;
}

template <typename T>
AnimatedList<T>::~AnimatedList() {
    //void
}
};
