#pragma once

#include "CSV_ControlsArea.h"

namespace base {
class CSV_HistoryList : public CSV_ControlsArea {
private: typedef CSV_ControlsArea super;
protected:
	static MenuItemConfig* new_menuItemConfigs_contentArea_historyArea();

public:
    CSV_HistoryList(CSV_HistoryList const&) = delete;
    CSV_HistoryList(CSV_HistoryList &&) = default;
    CSV_HistoryList& operator=(CSV_HistoryList const&) = delete;
    CSV_HistoryList& operator=(CSV_HistoryList &&) = default;

    explicit CSV_HistoryList(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal);

    ~CSV_HistoryList() override;
};
};
