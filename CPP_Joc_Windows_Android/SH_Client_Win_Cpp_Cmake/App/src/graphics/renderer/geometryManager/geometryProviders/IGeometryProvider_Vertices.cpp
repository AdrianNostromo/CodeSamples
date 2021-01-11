#include "IGeometryProvider_Vertices.h"

using namespace graphics;

int IGeometryProvider_Vertices::PROVIDER_TYPE = GetNew_PROVIDER_TYPE();

int IGeometryProvider_Vertices::getProviderType() {
	return PROVIDER_TYPE;
}

void* IGeometryProvider_Vertices::getProviderInterfaceVoidPointer() {
	return this;
}
