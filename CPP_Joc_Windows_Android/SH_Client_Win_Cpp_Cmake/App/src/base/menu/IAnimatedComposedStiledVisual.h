#pragma once

#include <functional>
#include <string>

class IGenericDrawable;

class IAnimatedComposedStiledVisual {
public:
    class IAnimationListener {
    public:
        virtual void animationFinished(IGenericDrawable* target) = 0;

        virtual ~IAnimationListener() = default;
    };
    class CbAnimationListener : virtual public IAnimationListener {
    public:
        std::function<void(
            IGenericDrawable* target)> cb_onAnimationFinished;

    public:
        CbAnimationListener(CbAnimationListener const&) = delete;
        CbAnimationListener(CbAnimationListener &&) = default;
        CbAnimationListener& operator=(CbAnimationListener const&) = delete;
        CbAnimationListener& operator=(CbAnimationListener &&) = default;

        explicit CbAnimationListener();

        void animationFinished(IGenericDrawable* target) override;

        ~CbAnimationListener() override;
    };

public:
	virtual std::string getManualStateFlags() = 0;
	virtual void setManualStateFlags(std::string& manualStateFlags, bool doAnimation) = 0;

	virtual ~IAnimatedComposedStiledVisual() = default;
};
