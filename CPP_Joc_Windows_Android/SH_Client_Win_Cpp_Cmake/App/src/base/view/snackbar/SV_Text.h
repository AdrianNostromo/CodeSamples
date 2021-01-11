#pragma once

#include <base/gh.h>
#include "SV_Base.h"

namespace base {
	class BitmapLabel;
};

namespace base {
namespace snackbar {
class SV_Text : public SV_Base {priv typedef SV_Base super;pub dCtor(SV_Text);
	prot static MenuItemConfig* new_menuItemConfigs_contentArea_textArea();

	prot base::BitmapLabel* contentArea_textArea_lblText = nullptr;

    pub explicit SV_Text(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler);

	prot void initializeSnackbarContent() override;

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;
	prot void syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) override;

	pub ~SV_Text() override;
};
};
};
