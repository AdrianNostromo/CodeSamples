#pragma once

#include <base/gh.h>

namespace rpg3D {
	class IEntityEditorView_Edit;
};

namespace rpg3D {
class IListenerEntityEditorView {
	pub class ButtonIds {
		pub static const int CLOSE;

	};

	pub virtual void onBtn(IEntityEditorView_Edit* target, int uiButtonId) = 0;

	pub virtual ~IListenerEntityEditorView() = default;
};
};
