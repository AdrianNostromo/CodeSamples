#include "CbListenerEntityEditorView.h"

using namespace rpg3D;

CbListenerEntityEditorView::CbListenerEntityEditorView() {
	//void
}

void CbListenerEntityEditorView::onBtn(IEntityEditorView_Edit* target, int uiButtonId) {
	if (cb_onBtn) {
		cb_onBtn(target, uiButtonId);
	}
}

CbListenerEntityEditorView::~CbListenerEntityEditorView() {
	//void
}
