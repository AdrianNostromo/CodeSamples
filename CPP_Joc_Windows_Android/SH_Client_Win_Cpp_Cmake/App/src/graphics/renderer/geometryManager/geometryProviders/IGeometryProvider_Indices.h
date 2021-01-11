#pragma once

#include <base/gh.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>

namespace graphics {
    class IndexBufferObject;
};

// The base class IGeometryProvider is not virtual because it must be used for each interface because this interface and similar can both be implemented on some classes and the base interface class code must be used for each provider type.
namespace graphics {
class IGeometryProvider_Indices : public /*virtual*/ IGeometryProvider {
	pub static int PROVIDER_TYPE;

    pub int getProviderType() final;
    prot void* getProviderInterfaceVoidPointer() final;

	pub virtual void* getIndicesData() = 0;
    pub virtual IndexBufferObject* getIndicesIBO() = 0;
    pub virtual int getIndicesOffset() = 0;
    pub virtual int getIndicesCount() = 0;

    pub ~IGeometryProvider_Indices() override = default;
};
};
