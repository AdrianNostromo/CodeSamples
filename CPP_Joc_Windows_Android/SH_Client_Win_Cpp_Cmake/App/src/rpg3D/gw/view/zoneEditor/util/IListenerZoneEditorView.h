#pragma once

#include <base/gh.h>

namespace rpg3D {
	class IZoneEditorView_Edit;
};

namespace rpg3D {
class IListenerZoneEditorView {
	pub class ButtonIds {
		pub static const int CLOSE;

	};

	pub virtual void onBtn(IZoneEditorView_Edit* target, int uiButtonId) = 0;

	pub virtual ~IListenerZoneEditorView() = default;
};
};
