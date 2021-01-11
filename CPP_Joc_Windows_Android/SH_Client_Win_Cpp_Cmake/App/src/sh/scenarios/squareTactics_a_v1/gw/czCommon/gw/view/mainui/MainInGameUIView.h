#pragma once

#include <base/gh.h>
#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include "IMainInGameUIView.h"

namespace squareTactics_a_v1 {
class MainInGameUIView : public rpg3D::MainInGameUIView, virtual public IMainInGameUIView {priv typedef rpg3D::MainInGameUIView super;pub dCtor(MainInGameUIView);
	prot static ArrayList<MenuItemConfig*>* viewItemConfigs;
	prot static MenuItemConfig* new_viewItemConfig_hp();

	priv base::BitmapLabel* hpLbl = nullptr;
	prot IInteractiveDrawable2D* btnEndPlayPhase;
	prot IDrawableNode2D* bgEndPlayPhase;

	priv rpg3D::ILivingEntityModule* livingEntity = nullptr;
	priv ArrayList<std::shared_ptr<IEventListener>> playerEntityListenersList{};

	priv int currentPlayerEntityHp = 0;
	priv int maxPlayerEntityHp = 0;

	priv CbTouchListener touchListener{};

    pub explicit MainInGameUIView(IApp* app, base::IGameWorld* gw);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	pub void visualLoop(float deltaS, int deltaMS) override;

	priv bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void onSelectedStateChanged() override;

	prot void onSelectedFocusEntityChange_pre() override;
	prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

	priv void onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

    pub ~MainInGameUIView() override;
};
};
