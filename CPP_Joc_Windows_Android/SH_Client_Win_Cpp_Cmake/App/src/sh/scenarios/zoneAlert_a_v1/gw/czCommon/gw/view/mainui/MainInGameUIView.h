#pragma once

#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include "IMainInGameUIView.h"

namespace zoneAlert_a_v1 {
class MainInGameUIView : public rpg3D::MainInGameUIView, virtual public IMainInGameUIView {
private: typedef rpg3D::MainInGameUIView super;
protected:
	static ArrayList<MenuItemConfig*>* viewItemConfigs;
	static MenuItemConfig* new_viewItemConfig_hp();

private:
	base::BitmapLabel* hpLbl = nullptr;

	rpg3D::ILivingEntityModule* livingEntity = nullptr;
	ArrayList<std::shared_ptr<IEventListener>> playerEntityListenersList{};

	int currentPlayerEntityHp = 0;
	int maxPlayerEntityHp = 0;

public:
    MainInGameUIView(MainInGameUIView const&) = delete;
    MainInGameUIView(MainInGameUIView &&) = default;
    MainInGameUIView& operator=(MainInGameUIView const&) = delete;
    MainInGameUIView& operator=(MainInGameUIView &&) = default;

    explicit MainInGameUIView(IApp* app, base::IGameWorld* gw);

    ~MainInGameUIView() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	void onSelectedStateChanged() override;

	void onSelectedFocusEntityChange_pre() override;
	void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

private:
	void onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

};
};
