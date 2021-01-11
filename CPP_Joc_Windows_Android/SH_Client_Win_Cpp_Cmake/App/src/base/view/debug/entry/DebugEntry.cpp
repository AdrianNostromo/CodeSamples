#include "DebugEntry.h"
#include "IHandler_DebugEntry.h"
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include "../entryUser/DebugEntryUser.h"
#include <graphics/visual2d/drawable/ToggleSwitch2D.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>

using namespace base;

DebugEntry::DebugEntry(IHandler_DebugEntry* handler, int pageIndex, int linesCount, BitmapLabel* lbl, base::ToggleSwitch2D* toggleSwitch, base::SpriteButton2D* btn)
	: handler(handler), pageIndex(pageIndex), linesCount(linesCount), lbl(lbl), toggleSwitch(toggleSwitch), btn(btn)
{
	toggleSwitchListener.cb_onSelectedStateChanged = std::bind(&DebugEntry::toggleSwitch_onSelectedStateChanged, this, std::placeholders::_1);
	btnTouchListener.cb_onCursorDown = std::bind(&DebugEntry::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void DebugEntry::toggleSwitch_onSelectedStateChanged(base::IToggleControl2D* target) {
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(toggleSwitch)) {
		if (hookedUser != nullptr && hookedUser->cb_onToggleSwitchStateChanged) {
			hookedUser->cb_onToggleSwitchStateChanged(hookedUser, target->getIsOn());
		}
	} else {
		throw LogicException(LOC);
	}
}

bool DebugEntry::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(btn)) {
		if (hookedUser != nullptr && hookedUser->cb_onBtnCursorDown) {
			hookedUser->cb_onBtnCursorDown(hookedUser);
		}
	}

	else {
		throw LogicException(LOC);
	}

	return true;
}

void DebugEntry::invalidateData() {
	if (!isDirty_data) {
		handler->onDebugEntryInvalidation(this);
	}
}

void DebugEntry::remove() {
	handler->removeDebugEntry(this);
}

void DebugEntry::setLinesCount(int linesCount) {
	if (this->linesCount == linesCount) {
		return;
	}

	this->linesCount = linesCount;

	handler->onDebugEntryLinesCountChanged(this);
}

void DebugEntry::setToggleSwitchIsOn(bool isOn) {
	if (toggleSwitch == nullptr) {
		throw LogicException(LOC);
	}

	toggleSwitch->setIsOn(isOn, true, false, false, false);
}

void DebugEntry::syncLblText() {
	if (lbl == nullptr) {
		throw LogicException(LOC);
	}

	if (hookedUser != nullptr) {
		std::string s = hookedUser->getText();
		lbl->setTextDirect(s);
	} else {
		lbl->setTextDirect("");
	}
}

DebugEntry::~DebugEntry() {
	//void
}
