#pragma once

#include <base/gh.h>
#include <string>
#include "IDebugEntry.h"
#include <graphics/visual2d/drawable/IToggleControl2D.h>
#include <base/interaction/CbTouchListener.h>

class IListEntry;
namespace base {
	class IHandler_DebugEntry;
	class BitmapLabel;
	class IDebugEntry;
	class DebugEntryUser;
	class IToggleControl2D;
	class ToggleSwitch2D;
	class SpriteButton2D;
};

namespace base {
class DebugEntry : virtual public IDebugEntry {pub dCtor(DebugEntry);
	pub IHandler_DebugEntry* handler;
	pub int pageIndex;

	pub int linesCount;

	pub BitmapLabel* lbl;
	
	pub base::ToggleSwitch2D* toggleSwitch;
	pub IListEntry* toggleSwitchLoopManager_listEntry = nullptr;
	pub base::IToggleControl2D::CbListener toggleSwitchListener{};

	pub base::SpriteButton2D* btn;
	pub CbTouchListener btnTouchListener{};

	pub bool isDirty_data = false;
	pub IListEntry* inHandler_invalidEntriesListEntry = nullptr;

	// This is a pointer of a s_p data.
	pub DebugEntryUser* hookedUser = nullptr;

	pub IListEntry* inHandler_listEntry = nullptr;

	pub explicit DebugEntry(IHandler_DebugEntry* handler, int pageIndex, int linesCount, BitmapLabel* lbl, base::ToggleSwitch2D* toggleSwitch, base::SpriteButton2D* btn);

	priv void toggleSwitch_onSelectedStateChanged(base::IToggleControl2D* target);

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub void invalidateData() final;

	pub void remove() final;

	pub void setLinesCount(int linesCount) final;
	pub void setToggleSwitchIsOn(bool isOn) final;

	pub void syncLblText();

	pub ~DebugEntry() override;
};
};
