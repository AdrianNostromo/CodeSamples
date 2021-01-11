#include "IRenderPhase2DFinalOutput.h"
#include <base/exceptions/LogicException.h>

using namespace graphics;

int IRenderPhase2DFinalOutput::TYPE = GetNew_TYPE();

int IRenderPhase2DFinalOutput::getType() {
	return TYPE;
}
