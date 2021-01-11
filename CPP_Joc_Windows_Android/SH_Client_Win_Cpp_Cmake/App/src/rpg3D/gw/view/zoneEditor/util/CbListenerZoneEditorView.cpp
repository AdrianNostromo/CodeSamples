#include "CbListenerZoneEditorView.h"

using namespace rpg3D;

CbListenerZoneEditorView::CbListenerZoneEditorView() {
	//void
}

void CbListenerZoneEditorView::onBtn(IZoneEditorView_Edit* target, int uiButtonId) {
	if (cb_onBtn) {
		cb_onBtn(target, uiButtonId);
	}
}

CbListenerZoneEditorView::~CbListenerZoneEditorView() {
	//void
}
