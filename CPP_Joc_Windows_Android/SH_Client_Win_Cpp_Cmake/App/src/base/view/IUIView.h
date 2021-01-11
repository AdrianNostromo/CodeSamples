#pragma once

#include <base/gh.h>
#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <base/view/util/ViewDataBase.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

class ViewEvent;
class IComponent;

class IUIView {
    pub class IHandlerAutoDisposal {
		pub virtual void onRequestDisposeView(IComponent* target) = 0;

		pub virtual ~IHandlerAutoDisposal() = default;
	};
    
    pub virtual void show(bool doAnimation, ViewDataBase* viewData, IAnimatedComposedStiledVisual::IAnimationListener* menuAnimationListener) = 0;
    pub virtual void show(bool doAnimation) = 0;
    pub virtual void hide(bool doAnimation, IAnimatedComposedStiledVisual::IAnimationListener* menuAnimationListener) = 0;
    pub virtual void hide(bool doAnimation) = 0;

    pub virtual base::TypedEventDispatcher1D<ViewEvent>& getEvents() = 0;

    pub virtual ~IUIView() = default;

};
