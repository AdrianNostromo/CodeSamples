#include "IColorSelectorView.h"

using namespace base;

IColorSelectorView::CbListener::CbListener() {
	//void
}

void IColorSelectorView::CbListener::onDynamicButtonActivated(IColorSelectorView* view, DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor) {
	if (cb_onDynamicButtonActivated != nullptr) {
		cb_onDynamicButtonActivated(view, dynamicButtonConfig, activeColor);
	}
}

void IColorSelectorView::CbListener::onCloseRequested(IColorSelectorView* view) {
	if (cb_onCloseRequested != nullptr) {
		cb_onCloseRequested(view);
	}
}

IColorSelectorView::CbListener::~CbListener() {
	//void
}
