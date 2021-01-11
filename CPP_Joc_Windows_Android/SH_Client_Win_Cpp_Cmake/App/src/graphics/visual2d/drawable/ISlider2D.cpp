#include "ISlider2D.h"

using namespace base;

ISlider2D::CbListener::CbListener() {
	//void
}

void ISlider2D::CbListener::onValueChanged(ISlider2D* target) {
	if (cb_onValueChanged) {
		cb_onValueChanged(target);
	}
}

ISlider2D::CbListener::~CbListener() {
	//void
}
