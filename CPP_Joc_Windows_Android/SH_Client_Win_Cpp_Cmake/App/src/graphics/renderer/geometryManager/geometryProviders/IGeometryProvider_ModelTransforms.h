#pragma once

#include <base/gh.h>
#include <graphics/renderer/geometryManager/geometryProviders/IGeometryProvider.h>

class Matrix4;
namespace graphics {
    class UniformBufferObject;
};

// The base class IGeometryProvider is not virtual because it must be used for each interface because this interface and similar can both be implemented on some classes and the base interface class code must be used for each provider type.
namespace graphics {
class IGeometryProvider_ModelTransforms : public /*virtual*/ IGeometryProvider {
    pub static int PROVIDER_TYPE;

    pub int getProviderType() final;
    prot void* getProviderInterfaceVoidPointer() final;

    pub virtual UniformBufferObject* getModelTransformsUBO() = 0;
    pub virtual UniformBufferObject* getRemapingModelTransformsIndicesUBOOptional() = 0;

    pub virtual Matrix4* getWorldTransformOptional() = 0;
    pub virtual Matrix4* getWorldTransformMustExist() = 0;

    pub virtual UniformBufferObject* getModelNormalTransformsUBO() = 0;

    // This is suposed to be a Matrix3 but when writing to opengl 3 rows of 4 floats are required because .
    /// Using a Matrix4 and writing 3*4 floats works with a single memcpy call.
    /// The right column and bottom row are not used (the right column is only written as padding).
    pub virtual Matrix4* getNormalWorldTransformOptional() = 0;
    pub virtual Matrix4* getNormalWorldTransformMustExist() = 0;

    pub ~IGeometryProvider_ModelTransforms() override = default;
};
};
