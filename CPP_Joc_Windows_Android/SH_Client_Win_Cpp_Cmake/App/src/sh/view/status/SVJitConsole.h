#pragma once

#include <base/interaction/CbTouchListener.h>
#include "SVBase.h"

namespace sh {
class SVJitConsole : public SVBase {
private: typedef SVBase super;
protected:
    static MenuItemConfig* new_viewItemConfig_jitConsoleScreenTa();
    static MenuItemConfig* new_viewItemConfig_jitConsoleScreenDimmer();

    static MenuItemConfig* new_viewItemConfig_console_header();
    static MenuItemConfig* new_viewItemConfig_console_body();
    static MenuItemConfig* new_viewItemConfig_console();

private:
    CbTouchListener touchListener{};

    IInteractiveDrawable2D* jitConsoleScreenTa;
    IInteractiveDrawable2D* header_ta;
    IInteractiveDrawable2D* body_ta;

public:
    SVJitConsole(SVJitConsole const&) = delete;
    SVJitConsole(SVJitConsole &&) = default;
    SVJitConsole& operator=(SVJitConsole const&) = delete;
    SVJitConsole& operator=(SVJitConsole &&) = default;

    explicit SVJitConsole(
        IAppSquareHeads* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    std::string computeStateFlags(std::string baseState) override;

    ~SVJitConsole() override;
protected:
    void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

private:
    bool v2d_onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos);

};
};
