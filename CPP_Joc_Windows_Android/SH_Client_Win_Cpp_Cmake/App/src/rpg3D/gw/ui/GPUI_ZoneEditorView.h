#pragma once

#include <base/gh.h>
#include "GPUIMainUIView.h"
#include <rpg3D/gw/view/zoneEditor/IZoneEditorView.h>
#include <rpg3D/gw/entity/module/zoneEditor/util/IHandlerZoneEditorModule.h>
#include <memory>

namespace base {
    class IDebugEntryUser;
};
namespace rpg3D {
    class ZoneEditorView;
    class Tool;
};

namespace rpg3D {
class GPUI_ZoneEditorView : public GPUIMainUIView, public virtual IHandlerZoneEditorModule {priv typedef GPUIMainUIView super;pub dCtor(GPUI_ZoneEditorView);
    priv struct DebugViewEntry_StartZoneEditor {
        std::shared_ptr<base::IDebugEntryUser> dde = nullptr;

    } debugViewEntry_StartZoneEditor;

    priv bool isEditorActive = false;
    priv ZoneEditorView* zoneEditorView = nullptr;
    priv IWorldEntity* editorEntity = nullptr;

    priv IZoneEditorView::CbAnimationListener viewHideAnimationListener{};

    priv sp<Tool> preEditorToolAtSlot = nullptr;

    pub explicit GPUI_ZoneEditorView(base::IGameWorld* gw);
    pub void createBMain() override;

    pub void onEvent(IEventListener& eventListener, base::GWEvent& event);

    priv void onStartZoneEditorBtnCursorDown(base::IDebugEntryUser* debugEntryUser);

    pub void startZoneEditor() override;
    priv ZoneEditorView* createNewView();

    pub void addEditorTools(IWorldEntity* editorEntity);
    pub void removeEditorTools(IWorldEntity* editorEntity);

    pub void zoneEditor_requestStopEditor() override;

    pub void stopZoneEditor(bool doAnimation) override;
    prot virtual void zoneEditor_onEditorClosed();

    priv void onViewHideAnimationFinished(IGenericDrawable* target);

    pub void disposePrePre() override;
    pub ~GPUI_ZoneEditorView() override;
};
};
