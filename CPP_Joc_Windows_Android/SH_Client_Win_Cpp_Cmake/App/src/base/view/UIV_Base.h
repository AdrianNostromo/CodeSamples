#pragma once

#include <base/menu/AnimatedComposedStiledVisual.h>
#include "IUIView.h"

class IApp;

class UIV_Base : public AnimatedComposedStiledVisual, virtual public IUIView {priv typedef AnimatedComposedStiledVisual super;pub dCtor(UIV_Base);
    prot IApp* app;

    // These callbacks will be auto disconnected on dispose.
    /// Note. If custom removal listenres are required, use a local list or individual containers.
    prot ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};

    pub explicit UIV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    pub ~UIV_Base() override;
};
