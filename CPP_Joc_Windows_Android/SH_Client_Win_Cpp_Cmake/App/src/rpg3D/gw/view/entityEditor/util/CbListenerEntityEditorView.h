#pragma once

#include <base/gh.h>
#include "IListenerEntityEditorView.h"
#include <functional>

namespace rpg3D {
	class IEntityEditorView_Edit;
};

namespace rpg3D {
class CbListenerEntityEditorView : virtual public IListenerEntityEditorView {pub dCtor(CbListenerEntityEditorView);
	pub std::function<void(
		IEntityEditorView_Edit * target, int uiButtonId)> cb_onBtn;

	pub explicit CbListenerEntityEditorView();

	pub void onBtn(IEntityEditorView_Edit* target, int uiButtonId) override;

	pub ~CbListenerEntityEditorView() override;
};
};
