#pragma once

#include <base/gh.h>
#include "IListenerZoneEditorView.h"
#include <functional>

namespace rpg3D {
	class IZoneEditorView_Edit;
};

namespace rpg3D {
class CbListenerZoneEditorView : virtual public IListenerZoneEditorView {pub dCtor(CbListenerZoneEditorView);
	pub std::function<void(
		IZoneEditorView_Edit * target, int uiButtonId)> cb_onBtn;

	pub explicit CbListenerZoneEditorView();

	pub void onBtn(IZoneEditorView_Edit* target, int uiButtonId) override;

	pub ~CbListenerZoneEditorView() override;
};
};
