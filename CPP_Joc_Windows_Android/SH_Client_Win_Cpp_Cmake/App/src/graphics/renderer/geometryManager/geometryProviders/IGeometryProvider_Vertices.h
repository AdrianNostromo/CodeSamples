#pragma once

#include <base/gh.h>
#include <memory>
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>

namespace graphics {
    class IGeometryManager;
    class GeometryProviderInstance;
    class Material;
    class VertexAttributesList;
    class VertexBufferObject;
};

// The base class IGeometryProvider is not virtual because it must be used for each interface because this interface and similar can both be implemented on some classes and the base interface class code must be used for each provider type.
namespace graphics {
class IGeometryProvider_Vertices : public /*virtual*/ IGeometryProvider {
    pub static int PROVIDER_TYPE;

    pub int getProviderType() final;
    prot void* getProviderInterfaceVoidPointer() final;

    //asd_02;// For now, keep the material in this vertices provider, maybe in the future use a separat eprovider for materials.
    pub virtual std::shared_ptr<graphics::Material> getMaterial() = 0;

    pub virtual std::shared_ptr<VertexAttributesList> getVertexAttributesList() = 0;
    pub virtual long getVertexAttributesBitMask() = 0;

    pub virtual void* getVerticesData() = 0;
    pub virtual VertexBufferObject* getVerticesVBO() = 0;
    pub virtual int getVerticesCount() = 0;

    pub ~IGeometryProvider_Vertices() override = default;
};
};
