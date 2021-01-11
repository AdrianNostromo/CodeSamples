#include "RS_Settings.h"

using namespace base;

RS_Settings::RS_Settings()
	: super()
{
	//void
}

bool RS_Settings::getSettingBool(std::string& settingId, bool notFoundValue) {
	std::shared_ptr<SettingEntry> settingEntry = settingsMap.getDirect(settingId, nullptr);

	if (settingEntry == nullptr || settingEntry->wValue == nullptr) {
		return notFoundValue;
	}

	return settingEntry->wValue->getDirectAs_bool();
}

std::shared_ptr<RS_Settings::SettingEntry> RS_Settings::getOrCreateSettingEntry(std::string& settingId) {
	std::shared_ptr<SettingEntry> settingEntry = settingsMap.getDirect(settingId, nullptr);

	if (settingEntry == nullptr) {
		settingEntry = std::make_shared<SettingEntry>();
		settingsMap.putDirect(settingId, settingEntry);
	}

	return settingEntry;
}

void RS_Settings::setSetting(std::string& settingId, bool value) {
	std::shared_ptr<SettingEntry> settingEntry = getOrCreateSettingEntry(settingId);

	bool isValueChanged = false;
	if (settingEntry->wValue == nullptr) {
		settingEntry->wValue = IWrappedValue::new_bool(value);

		isValueChanged = true;
	} else {
		if (settingEntry->wValue->getDirectAs_bool() != value) {
			settingEntry->wValue->getReferenceAs_bool() = value;

			isValueChanged = true;
		}
	}

	if (isValueChanged) {
		EventRuntimeSetting event{ settingId, settingEntry->wValue };

		settingEntry->dispatcher.dispatch(event);
	}
}

bool RS_Settings::clearSettingIfExists(std::string& settingId) {
	std::shared_ptr<SettingEntry> settingEntry = settingsMap.getDirect(settingId, nullptr);

	if (settingEntry != nullptr) {
		bool hadWValue = settingEntry->wValue != nullptr ? true : false;

		settingEntry->wValue = nullptr;

		if (settingEntry->dispatcher.getListenersCount() == 0) {
			// No liteners, can remove the entry.
			settingsMap.removeMustExist(settingId);
		} else {
			// Entry has listeners, keep it.

			if (hadWValue) {
				// Entry had a wValue, dispatch a nullptr wValue change.
				EventRuntimeSetting event{ settingId, nullptr };

				settingEntry->dispatcher.dispatch(event);
			}
		}

		return true;
	}

	return false;
}

std::shared_ptr<IEventListener> RS_Settings::addListener(
	std::string& settingId,
	std::function<void(IEventListener& eventListener, EventRuntimeSetting& event)> cb)
{
	std::shared_ptr<SettingEntry> settingEntry = getOrCreateSettingEntry(settingId);

	return settingEntry->dispatcher.addListener(cb, nullptr, nullptr, nullptr);
}

RS_Settings::~RS_Settings() {
	//void
}
