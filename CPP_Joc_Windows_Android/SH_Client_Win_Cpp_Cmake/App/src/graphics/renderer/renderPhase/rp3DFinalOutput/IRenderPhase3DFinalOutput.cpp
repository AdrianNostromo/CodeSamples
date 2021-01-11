#include "IRenderPhase3DFinalOutput.h"
#include <base/exceptions/LogicException.h>

using namespace graphics;

int IRenderPhase3DFinalOutput::TYPE = GetNew_TYPE();

int IRenderPhase3DFinalOutput::getType() {
	return TYPE;
}
