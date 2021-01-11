#pragma once

#include <base/interaction/CbTouchListener.h>
#include "MSVPanel.h"

namespace base {
	namespace audio {
		class ISound;
	};
};

namespace sh {
class MainSidebarView : public MSVPanel {priv typedef MSVPanel super; pub dCtor(MainSidebarView);
private:
    static ArrayList<MenuItemConfig*>* viewItemConfigs;
	
    static ArrayList<StateChangeDurations*>* viewAnimationDurations;

    IInteractiveDrawable2D* screenTa = nullptr;

	base::audio::ISound* soundConfigBgDown = nullptr;

    CbTouchListener touchListener{};
    CbAnimationListener menuAnimationListener{};

public:
    explicit MainSidebarView(
        IApp* app,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    ~MainSidebarView() override;
protected:
    void createContent() override;
    void onScreenTaTouchDown() override;

private:
    bool v2d_onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos);
	// This is currently not used because the view is never hidden with the new system.
    void onAnimationFinished(
        IGenericDrawable* target);
};
};
