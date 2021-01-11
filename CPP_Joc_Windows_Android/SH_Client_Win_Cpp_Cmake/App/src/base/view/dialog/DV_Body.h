#pragma once

#include "DV_Title.h"

namespace base {
class DV_Body : public DV_Title {
private: typedef DV_Title super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_bodyArea();

public:
    DV_Body(DV_Body const&) = delete;
    DV_Body(DV_Body &&) = default;
    DV_Body& operator=(DV_Body const&) = delete;
    DV_Body& operator=(DV_Body &&) = default;

    explicit DV_Body(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

    ~DV_Body() override;
protected:
	void initializeDialogAreas() override;
};
};
