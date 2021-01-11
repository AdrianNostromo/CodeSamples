#pragma once

#include <base/gh.h>
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include "GPUIBase.h"

namespace rpg3D {
class GPUIMainUIView : public GPUIBase {priv typedef GPUIBase super;pub dCtor(GPUIMainUIView);
	priv MIGUIV_Base::CbPanelHandler panelHandler{};

	priv MainInGameUIView* mainUiView = nullptr;

	// (>=1)// Active;
	// (<1)// Hidden;
	priv int showViewCounter = 0;
	
	// (>=1)// Active;
	// (<1)// Hidden;
	priv int mainUIMinimalModeCounter = 0;

	// This is here as a cache in case the ui is toggled or created late.
	priv IMainInGameUIView::IListenerDPad* mainUIListenerDPad = nullptr;
	priv IMainInGameUIView::IListenerToolExtraActivation* mainUIListenerToolExtraActivation = nullptr;

	pub explicit GPUIMainUIView(base::IGameWorld* gw);

	pub void showViewIncremental() final;
	pub void hideViewIncremental() final;
	
	pub void activateMainUIMinimalModeIncremental();
	pub void deactivateMainUIMinimalModeIncremental();

	pub void createBMain() override;

	pub void setMainUiDPadListener(IMainInGameUIView::IListenerDPad* listenerDPad) final;
	pub void setMainUiToolExtraActivationListener(IMainInGameUIView::IListenerToolExtraActivation* listenerToolExtraActivation) final;

	priv void onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event);

	pub void setNewMainUIViewUninitialised(MainInGameUIView* view) final;
	pub IMainInGameUIView* getMainUIView() override;

	prot void disposeMain() override;

	priv void ui_onRequestShowMainSidebarView();
	priv void ui_onRequestToggleCameraMode();
	priv void ui_onSetMoveDirection(float moveDirVectorX, float moveDirVectorY);
	priv void ui_onStopMovement();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	pub ~GPUIMainUIView() override;
};

};
