#pragma once

#include <base/gh.h>
#include "GPUI_EntityEditorView.h"

namespace startarea_a_v1 {
class GamePartUserInterface : public GPUI_EntityEditorView {priv typedef GPUI_EntityEditorView super;pub dCtor(GamePartUserInterface);
    pub explicit GamePartUserInterface(base::IGameWorld* gw);

    pub ~GamePartUserInterface() override;
};
};
