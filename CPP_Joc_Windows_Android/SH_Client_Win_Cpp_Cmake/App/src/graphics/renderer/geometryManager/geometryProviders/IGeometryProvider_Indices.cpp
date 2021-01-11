#include "IGeometryProvider_Indices.h"

using namespace graphics;

int IGeometryProvider_Indices::PROVIDER_TYPE = GetNew_PROVIDER_TYPE();

int IGeometryProvider_Indices::getProviderType() {
	return PROVIDER_TYPE;
}

void* IGeometryProvider_Indices::getProviderInterfaceVoidPointer() {
	return this;
}
