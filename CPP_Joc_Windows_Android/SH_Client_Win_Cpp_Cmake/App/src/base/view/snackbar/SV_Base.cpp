#include "SV_Base.h"
#include <base/app/IAppMetrics.h>
#include <base/view/snackbar/util/WrappedSnackbarView.h>
#include "inOrderDisplay/ISnackbarViewsInOrderDisplayHandler.h"
#include "util/Config.h"

using namespace base;
using namespace base::snackbar;

SV_Base::SV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal, ISnackbarViewsInOrderDisplayHandler* viewInOrderDisplayHandler)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal), viewInOrderDisplayHandler(viewInOrderDisplayHandler),
	wrapper(new WrappedSnackbarView(this))
{
	//void
}

Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* SV_Base::getUserDataMap() {
	if (userDataMap == nullptr) {
		userDataMap = new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>();
	}

	return userDataMap;
}

void SV_Base::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (isActive()) {
		activeDurationS += deltaS;
		if (activeDurationS >= config->autoCloseTimerS) {
			// The view will be auto disposed when hide finishes because it is configured as a auto-disposed view.
			hide(true, nullptr);
		}
	}
}

void SV_Base::showInOrderManaged(bool doAnimation, std::shared_ptr<Config> config) {
	this->config = config;

	viewInOrderDisplayHandler->addInOrderView(this, doAnimation);
}

IListEntry*& SV_Base::getInDisplayOrder_listEntryRef() {
	return _inDisplayOrder_listEntry;
}

bool SV_Base::getIsViewShowing() {
	return isActive();
}

void SV_Base::showView(bool doAnimation) {
	if (isActive() || config == nullptr) {
		// View is already active.

		throw LogicException(LOC);
	}

	initializeSnackbarContent();

	std::string stateID = computeStateFlags("_in_");
	if (!doAnimation || endStateID != stateID) {
		setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
	}
}

void SV_Base::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		if (_inDisplayOrder_listEntry != nullptr) {
			viewInOrderDisplayHandler->removeInOrderView(this);
		}
	} else {
		//void
	}
}

void SV_Base::initializeSnackbarContent() {
	//void
}

void SV_Base::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	getDataMap()->putDirectB("width", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("titleAreaHeight", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("bodyAreaHeight", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("controlsAreaHeight", base::IWrappedValue::new_float(0.0f));

	super::createVisuals(createConfigMap);
}

std::string SV_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (this->getIsTitleExists()) {
		state += "isTitleExists_";
	}

	if (this->getIsBodyExists()) {
		state += "isBodyExists_";
	}

	return state;
}

sp<WrappedSnackbarView> SV_Base::getWrapper() {
	return wrapper;
}

bool SV_Base::getIsTitleExists() {
	return isTitleExists;
}

void SV_Base::putIsTitleExists(bool isTitleExists, bool updateShowState) {
	if (this->isTitleExists == isTitleExists) {
		return;
	}

	this->isTitleExists = isTitleExists;

	eventChangeIsTitleExists();

	if (isActive() && updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

void SV_Base::eventChangeIsTitleExists() {
	//void
}

bool SV_Base::getIsBodyExists() {
	return isBodyExists;
}

void SV_Base::putIsBodyExists(bool isBodyExists, bool updateShowState) {
	if (this->isBodyExists == isBodyExists) {
		return;
	}

	this->isBodyExists = isBodyExists;

	eventChangeIsBodyExists();

	if (isActive() && updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

void SV_Base::eventChangeIsBodyExists() {
	//void
}

void SV_Base::dispose() {
	if (_inDisplayOrder_listEntry != nullptr) {
		viewInOrderDisplayHandler->removeInOrderView(this);
	}

	if (wrapper != nullptr) {
		wrapper->view = nullptr;
		wrapper = nullptr;
	}

	super::dispose();
}

SV_Base::~SV_Base() {
	
	//void
}
