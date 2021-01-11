#include "IToggleControl2D.h"

using namespace base;

IToggleControl2D::CbListener::CbListener() {
	//void
}

void IToggleControl2D::CbListener::onSelectedStateChanged(IToggleControl2D* target) {
	if (cb_onSelectedStateChanged) {
		cb_onSelectedStateChanged(target);
	}
}

IToggleControl2D::CbListener::~CbListener() {
	//void
}
