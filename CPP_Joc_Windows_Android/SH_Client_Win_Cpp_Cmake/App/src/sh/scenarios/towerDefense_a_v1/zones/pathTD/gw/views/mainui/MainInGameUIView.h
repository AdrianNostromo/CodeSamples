#pragma once

#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include "IMainInGameUIView.h"
#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>

class DrawableNode2D;
namespace base {
	class BitmapLabel;
};

namespace towerDefense_a_v1 {
namespace PathTD {
class MainInGameUIView : public rpg3D::MainInGameUIView, virtual public IMainInGameUIView {priv typedef rpg3D::MainInGameUIView super;pub dCtor(MainInGameUIView);
    prot static ArrayList<MenuItemConfig*>* viewItemConfigs;
	priv static MenuItemConfig* new_viewItemConfig_top();

	priv CbTouchListener touchListener{};

	priv base::BitmapLabel* livesLbl = nullptr;
	priv base::BitmapLabel* goldLbl = nullptr;
	priv DrawableNode2D* startWaveBg = nullptr;
	priv DrawableNode2D* layerStartWave = nullptr;

	priv TouchArea2D* startWaveTA;

	priv std::function<void(IMainInGameUIView * target)> cb_onStartNextWaveRequest = nullptr;

	priv int currentLives = 0;
	priv int currentGold = 0;
	priv bool isWaveStarterBtnActive = false;

    pub explicit MainInGameUIView(IApp* app, base::IGameWorld* gw);
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub void setCb_onStartNextWaveRequest(std::function<void(
		IMainInGameUIView * target)> cb_onStartNextWaveRequest) final;

	pub void setCurrentLives(int currentLives) override;
	pub void setCurrentGold(int currentGold) override;
	pub void setIsWaveStarterBtnActive(bool isWaveStarterBtnActive) override;

	priv bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub void visualLoop(float deltaS, int deltaMS) override;

	pub std::string computeStateFlags(std::string baseState) override;
	pub ~MainInGameUIView() override;
};
};
};
