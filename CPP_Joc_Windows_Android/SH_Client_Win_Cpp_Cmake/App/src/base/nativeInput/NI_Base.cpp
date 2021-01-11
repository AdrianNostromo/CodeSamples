#include "NI_Base.h"
#include <base/exceptions/LogicException.h>

using namespace base;

NI_Base::NI_Base() {
	//void
}

void NI_Base::setScreenSize(int width, int height) {
	screenSize.set(width, height);
}

bool NI_Base::getIsHapticFeedbackSupported() {
	return false;
}

void NI_Base::doHapticFeedback(int hapticFeedbackType) {
	throw LogicException(LOC);
}

NI_Base::~NI_Base() {
	//void
}
