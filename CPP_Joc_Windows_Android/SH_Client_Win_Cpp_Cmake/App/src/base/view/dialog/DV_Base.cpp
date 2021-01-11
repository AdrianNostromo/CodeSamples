#include "DV_Base.h"
#include <base/app/IAppMetrics.h>

using namespace base;

DV_Base::DV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* DV_Base::getUserDataMap() {
	if (userDataMap == nullptr) {
		userDataMap = new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>();
	}

	return userDataMap;
}

void DV_Base::show(bool doAnimation, IAnimationListener* menuAnimationListener, std::shared_ptr<Config> dialogConfig) {
	if (isActive()) {
		// View is already active.

		throw LogicException(LOC);
	}

	this->dialogConfig = dialogConfig;

	initializeDialogAreas();

	std::string stateID = computeStateFlags("_in_");
	if (!doAnimation || endStateID != stateID) {
		setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
	}
}

void DV_Base::setListener(IAutoListenerDialogView* listener) {
	if (this->listener != nullptr) {
		removeListener(this->listener);
	}

	if (listener != nullptr) {
		this->listener = listener;

		addListener(listener);
	}
}

void DV_Base::removeListener(IAutoListener* listener) {
	if (this->listener != listener) {
		throw LogicException(LOC);
	}

	this->listener = nullptr;
	
	super::removeListener(listener);
}

void DV_Base::initializeDialogAreas() {
	getDataMap()->putDirectB("width", base::IWrappedValue::new_float(this->dialogConfig->widthDPM * appMetrics->getScreen()->getScreenDensityMultiplier()->x));
}

void DV_Base::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	getDataMap()->putDirectB("width", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("titleAreaHeight", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("bodyAreaHeight", base::IWrappedValue::new_float(0.0f));
	getDataMap()->putDirectB("controlsAreaHeight", base::IWrappedValue::new_float(0.0f));

	super::createVisuals(createConfigMap);
}

std::string DV_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (this->getIsTitleExists()) {
		state += "isTitleExists_";
	}

	if (this->getIsBodyExists()) {
		state += "isBodyExists_";
	}

	if (this->getIsControlsExists()) {
		state += "isControlsExists_";
	}

	return state;
}

bool DV_Base::getIsTitleExists() {
	return isTitleExists;
}

void DV_Base::putIsTitleExists(bool isTitleExists, bool updateShowState) {
	if (this->isTitleExists == isTitleExists) {
		return;
	}

	this->isTitleExists = isTitleExists;

	eventChangeIsTitleExists();

	if (isActive() && updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

void DV_Base::eventChangeIsTitleExists() {
	//void
}

bool DV_Base::getIsBodyExists() {
	return isBodyExists;
}

void DV_Base::putIsBodyExists(bool isBodyExists, bool updateShowState) {
	if (this->isBodyExists == isBodyExists) {
		return;
	}

	this->isBodyExists = isBodyExists;

	eventChangeIsBodyExists();

	if (isActive() && updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

void DV_Base::eventChangeIsBodyExists() {
	//void
}

bool DV_Base::getIsControlsExists() {
	return isControlsExists;
}

void DV_Base::putIsControlsExists(bool isControlsExists, bool updateShowState) {
	if (this->isControlsExists == isControlsExists) {
		return;
	}

	this->isControlsExists = isControlsExists;

	eventChangeIsControlsExists();

	if (isActive() && updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

void DV_Base::eventChangeIsControlsExists() {
	//void
}

DV_Base::~DV_Base() {
	//void
}
