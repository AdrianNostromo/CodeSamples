#include "CbAutoListenerDialogView.h"

using namespace base;

CbAutoListenerDialogView::CbAutoListenerDialogView() {
	//void
}

void CbAutoListenerDialogView::onDynamicButtonActivated(IDialogView* view, DynamicTextButtonConfig* dynamicButtonConfig) {
	if (cb_onDynamicButtonActivated != nullptr) {
		cb_onDynamicButtonActivated(view, dynamicButtonConfig);
	}
}

void CbAutoListenerDialogView::onCloseRequested(IDialogView* view) {
	if (cb_onCloseRequested != nullptr) {
		cb_onCloseRequested(view);
	}
}

void CbAutoListenerDialogView::onContentAreaTouched(IDialogView* view) {
	if (cb_onContentAreaTouched != nullptr) {
		cb_onContentAreaTouched(view);
	}
}

CbAutoListenerDialogView::~CbAutoListenerDialogView() {
	//void
}
