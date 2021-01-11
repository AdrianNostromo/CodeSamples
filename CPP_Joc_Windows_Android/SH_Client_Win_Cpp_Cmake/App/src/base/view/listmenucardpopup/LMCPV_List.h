#pragma once

#include "LMCPV_Base.h"
#include <graphics/visual2d/drawable/List2D.h>

namespace base {
class LMCPV_List : public LMCPV_Base {
private: typedef LMCPV_Base super;
private:
	List2D* optionsList = nullptr;

	List2D::CbListener listEventListener{};

protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_optionsList();

public:
    LMCPV_List(LMCPV_List const&) = delete;
    LMCPV_List(LMCPV_List &&) = default;
    LMCPV_List& operator=(LMCPV_List const&) = delete;
    LMCPV_List& operator=(LMCPV_List &&) = default;

    explicit LMCPV_List(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

	ListEntry2D* createNewListEntry(int index, ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap) override;

    ~LMCPV_List() override;
protected:
	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;
	
	void onSelectedStateChanged() override;

private:
	void optionsList_onEntryTouched(List2D* list, ListEntry2D* listItem);
	void helperCallListEntryActivated(List2D* list, ListEntry2D* listEntry);

};
};
