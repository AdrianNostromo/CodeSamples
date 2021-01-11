#include "DebugEntryUser.h"
#include "../entry/IDebugEntry.h"
#include <base/util/StringUtil.h>

using namespace base;

DebugEntryUser::DebugEntryUser(IDebugEntry* debugEntry)
	: debugEntry(debugEntry)
{
	//void
}

bool DebugEntryUser::getIsConnected() {
	return debugEntry != nullptr ? true : false;
}

void DebugEntryUser::remove() {
	if (debugEntry != nullptr) {
		debugEntry->remove();
	}
}

void DebugEntryUser::setLinesCount(int linesCount) {
	debugEntry->setLinesCount(linesCount);
}

void DebugEntryUser::setPlaceholderedText(std::string placeholderedText) {
	this->placeholderedText = placeholderedText;

	invalidateData();
}

void DebugEntryUser::appendTrackedDataValue(std::shared_ptr<base::IWrappedValue> trackedWrappedValue) {
	wrappedValuesList.appendDirect(trackedWrappedValue);

	invalidateData();
}

void DebugEntryUser::invalidateData() {
	if (debugEntry != nullptr) {
		debugEntry->invalidateData();
	}
}

void DebugEntryUser::setToggleSwitchIsOn(bool isOn) {
	if (debugEntry != nullptr) {
		debugEntry->setToggleSwitchIsOn(isOn);
	}
}

void DebugEntryUser::setCb_onToggleSwitchStateChanged(
	std::function<void(
		IDebugEntryUser* debugEntryUser, bool newState)> cb_onToggleSwitchStateChanged)
{
	this->cb_onToggleSwitchStateChanged = cb_onToggleSwitchStateChanged;
}

void DebugEntryUser::setCb_onBtnCursorDown(
	std::function<void(
		IDebugEntryUser* debugEntryUser)> cb_onBtnCursorDown)
{
	this->cb_onBtnCursorDown = cb_onBtnCursorDown;
}

std::string DebugEntryUser::getText() {
	std::string s = "";
	s = placeholderedText;

	for (int i = 0; i < wrappedValuesList.count(); i++) {
		std::shared_ptr<base::IWrappedValue>& wrappedValue = wrappedValuesList.getReference(i);

		int occurencesCount = 0;
		std::string replacePattern = "#" + std::to_string(i) + "#";
		if (wrappedValue->checkType(base::IWrappedValue::Type::T_int)) {
			occurencesCount = StringUtil::replace(s, replacePattern, std::to_string(wrappedValue->getDirectAs_int()));
		} else if (wrappedValue->checkType(base::IWrappedValue::Type::T_float)) {
			occurencesCount = StringUtil::replace(s, replacePattern, StringUtil::floatToFixedPrecisionString(wrappedValue->getDirectAs_float(), 2));
		} else {
			throw LogicException(LOC);
		}
		if (occurencesCount != 1) {
			// Replace not made or made more than once.
			throw LogicException(LOC);
		}
	}

	return s;
}

DebugEntryUser::~DebugEntryUser() {
	if (debugEntry != nullptr) {
		remove();
	}
}
