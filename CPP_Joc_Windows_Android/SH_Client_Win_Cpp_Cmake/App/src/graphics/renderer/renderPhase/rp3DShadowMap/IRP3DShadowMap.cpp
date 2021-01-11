#include "IRP3DShadowMap.h"
#include <base/exceptions/LogicException.h>

using namespace graphics;

int IRP3DShadowMap::TYPE = GetNew_TYPE();

int IRP3DShadowMap::getType() {
	return TYPE;
}
