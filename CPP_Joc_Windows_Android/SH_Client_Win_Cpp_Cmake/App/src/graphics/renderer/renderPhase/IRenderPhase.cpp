#include "IRenderPhase.h"

using namespace graphics;

int IRenderPhase::GetNew_TYPE() {
	static int TYPE_COUNTER = 0;

	int ret = TYPE_COUNTER;
	TYPE_COUNTER++;

	return ret;
}
