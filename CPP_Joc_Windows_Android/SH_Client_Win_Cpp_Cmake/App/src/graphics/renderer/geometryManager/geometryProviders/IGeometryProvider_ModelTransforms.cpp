#include "IGeometryProvider_ModelTransforms.h"

using namespace graphics;

int IGeometryProvider_ModelTransforms::PROVIDER_TYPE = GetNew_PROVIDER_TYPE();

int IGeometryProvider_ModelTransforms::getProviderType() {
	return PROVIDER_TYPE;
}

void* IGeometryProvider_ModelTransforms::getProviderInterfaceVoidPointer() {
	return this;
}
