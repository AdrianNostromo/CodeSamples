#pragma once

#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include "SVJitConsole.h"

namespace sh {
class SVJitIndicator : public SVJitConsole {
private: typedef SVJitConsole super;
protected:
    static MenuItemConfig* new_viewItemConfig_indicator();

private:
    CbTouchListener touchListener{};

    IInteractiveDrawable2D* taIndicator;

public:
    SVJitIndicator(SVJitIndicator const&) = delete;
    SVJitIndicator(SVJitIndicator &&) = default;
    SVJitIndicator& operator=(SVJitIndicator const&) = delete;
    SVJitIndicator& operator=(SVJitIndicator &&) = default;

    explicit SVJitIndicator(
        IAppSquareHeads* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    std::string computeStateFlags(std::string baseState) override;

    ~SVJitIndicator() override;
protected:
    void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

private:
    bool v2d_onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos);
};
};
