#pragma once

#include "MSVPHeader.h"
#include <graphics/visual2d/drawable/IToggleControl2D.h>
#include <base/interaction/CbTouchListener.h>

namespace base {
	class BitmapLabel;
	class SpriteButton2D;
};

namespace sh {
class MSVPBody : public MSVPHeader {priv typedef MSVPHeader super; pub dCtor(MSVPBody);
	priv static const float SPACE_M;
	priv static const float SPACE_S;
	priv static const float TEXT_OFFSET_Y;
	priv static const float TITLE_OFFSET_Y;
	priv static const float TITLE_H;
	priv static const float ENTRY_H;
	priv static const float BTN_W;

	prot static MenuItemConfig* new_viewItemConfig_panel_body();
    
	priv CbTouchListener touchListener{};
	priv base::IToggleControl2D::CbListener toggleSwitchListener{};
	
	priv base::IToggleControl2D* tbtnAppSound = nullptr;
	priv base::IToggleControl2D* tbtnOnScreenDPad = nullptr;
	
	priv base::BitmapLabel* dlblCameraZoom = nullptr;
	
	priv base::SpriteButton2D* btnHome = nullptr;
	priv base::SpriteButton2D* btnExitZone = nullptr;
	
	priv base::SpriteButton2D* btnCameraZoomOut = nullptr;
	priv base::SpriteButton2D* btnCameraZoomIn = nullptr;

	priv base::BitmapLabel* dlblCameraDirection = nullptr;
	priv base::SpriteButton2D* btnCameraDirectionTurnLeft = nullptr;
	priv base::SpriteButton2D* btnCameraDirectionTurnRight = nullptr;

	priv base::BitmapLabel* dlblAppVersion = nullptr;

	pub explicit MSVPBody(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	priv void v2d_onSelectedStateChanged(base::IToggleControl2D* target);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	prot void onSelectedStateChanged() override;

	prot void isOnDisplayListChanged() override;

    pub ~MSVPBody() override;
};
};
