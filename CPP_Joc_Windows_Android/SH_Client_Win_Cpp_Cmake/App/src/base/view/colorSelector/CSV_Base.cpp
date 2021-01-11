#include "CSV_Base.h"
#include <base/util/StringUtil.h>

using namespace base;

CSV_Base::CSV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* CSV_Base::getUserDataMap() {
	if (userDataMap == nullptr) {
		userDataMap = new Map1D<std::string, std::shared_ptr<base::IWrappedValue>>();
	}

	return userDataMap;
}

void CSV_Base::show(
	bool doAnimation, IAnimationListener* menuAnimationListener, Color* initialColor, 
	std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs, 
	bool isAlphaEditable, int alphaOverrideHexValue)
{
	if (StringUtil::contains(endStateID, "_in_")) {
		// View is already active.

		throw LogicException(LOC);
	}

	this->dynamicButtonConfigs = dynamicButtonConfigs;

	setIsAlphaEditable(isAlphaEditable, alphaOverrideHexValue, false);

	std::string stateID = computeStateFlags("_in_");
	if (!doAnimation || endStateID != stateID) {
		setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
	}
	
	if (initialColor != nullptr) {
		setActiveColor(*initialColor, true, false);
	} else {
		// Set the color to the last one.
		//asd_001;
		Color col("#ff44AAFF");
		setActiveColor(col, true, false);
	}
}

void CSV_Base::setListener(IListener* listener) {
	if (this->listener != nullptr) {
		removeListener(this->listener);
	}

	if (listener != nullptr) {
		this->listener = listener;

		addListener(listener);
	}
}

void CSV_Base::removeListener(IAutoListener* listener) {
	if (this->listener != listener) {
		throw LogicException(LOC);
	}

	this->listener = nullptr;

	super::removeListener(listener);
}

std::string CSV_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (this->getIsHistoryExists()) {
		state += "isHistoryExists_";
	}

	if (this->getIsHistoryActive()) {
		state += "historyActive_";
	}

	if (this->getIsAlphaEditable()) {
		state += "isAlphaEditable_";
	}

	return state;
}

bool CSV_Base::getIsHistoryActive() {
	return isHistoryActive;
}

void CSV_Base::setIsHistoryActive(bool isHistoryActive, bool updateShowState) {
	if (this->isHistoryActive == isHistoryActive) {
		return;
	}

	this->isHistoryActive = isHistoryActive;

	if (updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

bool CSV_Base::getIsAlphaEditable() {
	return isAlphaEditable;
}

int CSV_Base::getAlphaOverrideHexValue() {
	return alphaOverrideHexValue;
}

void CSV_Base::setIsAlphaEditable(bool isAlphaEditable, int alphaOverrideHexValue, bool updateShowState) {
	if (this->isAlphaEditable == isAlphaEditable) {
		return;
	}

	this->isAlphaEditable = isAlphaEditable;
	this->alphaOverrideHexValue = alphaOverrideHexValue;

	if (updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

bool CSV_Base::getIsHistoryExists() {
	return isHistoryExists;
}

void CSV_Base::setIsHistoryExists(bool isHistoryExists, bool updateShowState) {
	if (this->isHistoryExists == isHistoryExists) {
		return;
	}

	this->isHistoryExists = isHistoryExists;

	if (updateShowState) {
		updateShowStateIfChanged(true, nullptr);
	}
}

Color* CSV_Base::getActiveColor() {
	return &activeColor;
}

void CSV_Base::setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders) {
	this->activeColor.set(activeColor);
}

CSV_Base::~CSV_Base() {
	if (userDataMap != nullptr) {
		delete userDataMap;

		userDataMap = nullptr;
	}
}
