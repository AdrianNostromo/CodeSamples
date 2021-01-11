#pragma once

#include "DV_Base.h"

namespace base {
class DV_Title : public DV_Base {
private: typedef DV_Base super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_titleArea();

public:
    DV_Title(DV_Title const&) = delete;
    DV_Title(DV_Title &&) = default;
    DV_Title& operator=(DV_Title const&) = delete;
    DV_Title& operator=(DV_Title &&) = default;

    explicit DV_Title(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

    ~DV_Title() override;
protected:
	void initializeDialogAreas() override;

};
};
