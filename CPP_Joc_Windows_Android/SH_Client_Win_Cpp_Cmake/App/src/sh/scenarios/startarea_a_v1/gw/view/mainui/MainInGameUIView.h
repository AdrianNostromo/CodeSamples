#pragma once

#include <base/interaction/CbTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include "IMainInGameUIView.h"

namespace startarea_a_v1 {
class MainInGameUIView : public rpg3D::MainInGameUIView, virtual public IMainInGameUIView {
private: typedef rpg3D::MainInGameUIView super;
protected:
    static ArrayList<MenuItemConfig*>* viewItemConfigs;

public:
    MainInGameUIView(MainInGameUIView const&) = delete;
    MainInGameUIView(MainInGameUIView &&) = default;
    MainInGameUIView& operator=(MainInGameUIView const&) = delete;
    MainInGameUIView& operator=(MainInGameUIView &&) = default;

    explicit MainInGameUIView(IApp* app, base::IGameWorld* gw);

    ~MainInGameUIView() override;
};
};
