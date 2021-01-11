#pragma once

#include "GPUI_EntityEditorView.h"

namespace rpg3D {
class GamePartUserInterface : public GPUI_EntityEditorView {
private: typedef GPUI_EntityEditorView super;
public:
    GamePartUserInterface(GamePartUserInterface const&) = delete;
    GamePartUserInterface(GamePartUserInterface &&) = default;
    GamePartUserInterface& operator=(GamePartUserInterface const&) = delete;
    GamePartUserInterface& operator=(GamePartUserInterface &&) = default;

    explicit GamePartUserInterface(base::IGameWorld* gw);

    ~GamePartUserInterface() override;
};
};
