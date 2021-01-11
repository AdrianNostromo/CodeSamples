#include "IListMenuCardPopupView.h"

using namespace base;

IListMenuCardPopupView::CbListener::CbListener() {
	//void
}

void IListMenuCardPopupView::CbListener::onListEntryActivated(IListMenuCardPopupView* targetView, List2D* list, ListEntry2D* listEntry) {
	if (cb_onListEntryActivated != nullptr) {
		cb_onListEntryActivated(targetView, list, listEntry);
	}
}

void IListMenuCardPopupView::CbListener::onCloseRequested(IListMenuCardPopupView* targetView) {
	if (cb_onCloseRequested != nullptr) {
		cb_onCloseRequested(targetView);
	}
}

IListMenuCardPopupView::CbListener::~CbListener() {
	//void
}
