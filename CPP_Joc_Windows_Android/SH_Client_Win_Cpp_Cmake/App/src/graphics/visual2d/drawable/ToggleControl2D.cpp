#include "ToggleControl2D.h"
#include <base/app/IAppAssets.h>

using namespace base;

ToggleControl2D::ToggleControl2D(IAppMetrics* appMetrics, IAppAssets* appAssets, ArrayList<MenuItemConfig*>* itemConfigs, ArrayList<StateChangeDurations*>* easeDurationsSList)
	: super(appMetrics, appAssets, itemConfigs, easeDurationsSList)
{
	touchListener.cb_onCursorDown = std::bind(&ToggleControl2D::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool ToggleControl2D::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (!isEnabled) {
		return false;
	}

	if (target == touchAreaBack) {
		setIsOn(!getIsOn(), true, true, true, true);
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void ToggleControl2D::setEventListener(IListener* eventListener) {
	this->eventListener = eventListener;
}

void ToggleControl2D::createContent() {
	super::createContent();

	toggleOffDSound = new base::audio::DynamicSound(appAssets->getEmbeddedAssetsManager());
	toggleOnDSound = new base::audio::DynamicSound(appAssets->getEmbeddedAssetsManager());
}

void ToggleControl2D::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(touchAreaBack = getItemMustExistAs< IInteractiveDrawable2D*>("touchAreaBack"))->addTouchListener(&touchListener);
}

bool ToggleControl2D::getIsOn() {
	return isOn;
}

void ToggleControl2D::setIsOn(bool isOn, bool doUpdate, bool doAnimation, bool playSound, bool dispatchEvent) {
	if (this->isOn == isOn) {
		return;
	}

	this->isOn = isOn;

	if (doUpdate) {
		updateShowStateIfChanged(doAnimation, nullptr);
	}

	if (dispatchEvent) {
		if (eventListener != nullptr) {
			eventListener->onSelectedStateChanged(this);
		}
	}

	if (playSound) {
		if (!isOn) {
			// Play off sound.

			if (toggleOffDSound != nullptr) {
				toggleOffDSound->play(toggleOffSound_volume);
			}
		} else {
			// Play on sound.

			if (toggleOnDSound != nullptr) {
				toggleOnDSound->play(toggleOnSound_volume);
			}
		}
	}
}

void ToggleControl2D::setIsEnabled(bool isEnabled, bool doUpdate, bool doAnimation) {
	if (this->isEnabled == isEnabled) {
		return;
	}

	this->isEnabled = isEnabled;

	if (doUpdate) {
		updateShowStateIfChanged(doAnimation, nullptr);
	}
}

void ToggleControl2D::setPosition(float x, float y) {
	// This function is called only when the toggle switch is manually created by code.
	// In that case, need to let the template logic know the values where initialised.
	// Note. make sure to call before reservedCreate is called. Don't call here because they need to be set before the reservedCreate.
	// putStaticAnimatingPropertyStatusDirect("x", true);
	// putStaticAnimatingPropertyStatusDirect("y", true);

	if (this->visualsHolder->getX() == x && this->visualsHolder->getY() == y) {
		return;
	}

	this->visualsHolder->setPosition(x, y);
}

void ToggleControl2D::setToggleOffSound(std::string* configId, float volume) {
	toggleOffDSound->setSound_id(configId);
	toggleOffSound_volume = volume;
}

void ToggleControl2D::setToggleOnSound(std::string* configId, float volume) {
	toggleOnDSound->setSound_id(configId);
	toggleOnSound_volume = volume;
}

std::shared_ptr<base::IWrappedValue> ToggleControl2D::getPropertyValue(std::string property) {
	if (property == "x") {
		return base::IWrappedValue::new_float(visualsHolder->getX());
	} else if (property == "y") {
		return base::IWrappedValue::new_float(visualsHolder->getY());
	}

	else if (property == "toggleOffSoundConfig_id") {
		return base::IWrappedValue::new_String_ptr(toggleOffDSound->getSound_id());
	} else if (property == "toggleOffSound_volume") {
		return base::IWrappedValue::new_float(toggleOffSound_volume);
	}

	else if (property == "toggleOnSoundConfig_id") {
		return base::IWrappedValue::new_String_ptr(toggleOnDSound->getSound_id());
	} else if (property == "toggleOnSound_volume") {
		return base::IWrappedValue::new_float(toggleOnSound_volume);
	}

	throw LogicException(LOC);
}

void ToggleControl2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	if (property == "x") {
		visualsHolder->setX(value->getDirectAs_float());
	} else if (property == "y") {
		visualsHolder->setY(value->getDirectAs_float());
	}

	else if (property == "toggleOffSoundConfig_id") {
		toggleOffDSound->setSound_id(value->getDirectAs_String_ptr());
	}else if (property == "toggleOffSound_volume") {
		toggleOffSound_volume = value->getDirectAs_float();
	}

	else if (property == "toggleOnSoundConfig_id") {
		toggleOnDSound->setSound_id(value->getDirectAs_String_ptr());
	}else if (property == "toggleOnSound_volume") {
		toggleOnSound_volume = value->getDirectAs_float();
	}

	else {
		throw LogicException(LOC);
	}
}

std::string ToggleControl2D::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (isEnabled) {
		state += "enabled_";
	} else {
		state += "disabled_";
	}

	if (isOn) {
		state += "on_";
	} else {
		state += "off_";
	}

	return state;
}

ToggleControl2D::~ToggleControl2D() {
	//void
}
