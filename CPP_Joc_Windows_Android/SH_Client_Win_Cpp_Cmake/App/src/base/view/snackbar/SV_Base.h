#pragma once

#include <base/gh.h>
#include <base/view/UIView.h>
#include "ISnackbarView.h"
#include "inOrderDisplay/util/IInOrderView.h"

namespace base {
namespace snackbar {
	class ISnackbarViewsInOrderDisplayHandler;
	class Config;
};
};

namespace base {
namespace snackbar {
class SV_Base : public UIView, virtual public ISnackbarView, virtual public IInOrderView {priv typedef UIView super;pub dCtor(SV_Base);
	prot std::shared_ptr<Config> config = nullptr;

	prot ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler;
	// NOTE. DO NOT MODIFY FROM HERE.
	priv IListEntry* _inDisplayOrder_listEntry = nullptr;

	priv bool isTitleExists = false;
	priv bool isBodyExists = false;

	// dataMap entries:
	// dynamic: int width
	// dynamic: int titleAreaHeight
	// dynamic: int bodyAreaHeight
	// dynamic: int controlsAreaHeight

	priv Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* userDataMap = nullptr;

	priv sp<WrappedSnackbarView> wrapper;

	priv float activeDurationS = 0.0f;

    pub explicit SV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler);

	pub void visualLoop(float deltaS, int deltaMS) override;

	pub Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap();

	pub void showInOrderManaged(bool doAnimation, std::shared_ptr<Config> config) final;

	pub IListEntry*& getInDisplayOrder_listEntryRef() final;

	pub bool getIsViewShowing() final;

	pub using super::show;
	pub void showView(bool doAnimation) final;

	pub std::string computeStateFlags(std::string baseState) override;

	pub sp<WrappedSnackbarView> getWrapper() final;

	prot void onSelectedStateChanged() override;

	prot virtual void initializeSnackbarContent();

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	prot bool getIsTitleExists();
	prot void putIsTitleExists(bool isTitleExists, bool updateShowState);
	prot void eventChangeIsTitleExists();
	prot bool getIsBodyExists();
	prot void putIsBodyExists(bool isBodyExists, bool updateShowState);
	prot void eventChangeIsBodyExists();

	void dispose() override;
	pub ~SV_Base() override;
};
};
};
