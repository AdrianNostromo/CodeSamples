#pragma once

#include <base/gh.h>
#include <assert.h>
#include <base/list/ArrayList.h>
#include <string>
#include <base/opengl/util/GLUtil.h>

namespace graphics {
    class UniformAttribute;
    class ShaderProgram;
};

namespace graphics {
class UniformAttributesList {pub dCtor(UniformAttributesList);
    pub class Type {
        pub static UniformAttribute* const projViewMatrix;
        pub static UniformAttribute* const modelTransform;
        // This is a Matrix3 and that requires 3 rows of 4 floats (1 float is padding).
        // This requires a special Matrix3GLPadded that contains the padding internaly.
        pub static UniformAttribute* const modelNormalTransform;
        pub static UniformAttribute* const uvec4;

        pub static UniformAttribute* const Padding1;
        pub static UniformAttribute* const Padding2;
        pub static UniformAttribute* const Padding3;
        pub static UniformAttribute* const Padding4;
    };

    // Unmanaged.
    priv ArrayList<UniformAttribute*> attributesList{};
    priv unsigned int stride = 0;
    priv long typesBitMask = 0;

    // This is used to make this list reusable after it is created.
    priv bool isLocked = false;

    pub UniformAttributesList();

    pub unsigned int getStride();
    pub unsigned int getTypesBitMask();

    pub bool hasAll(long typeBitMask);
    pub void pushAttribute(UniformAttribute* attribute);

    pub int getByteOffset(UniformAttribute* attribute);

    pub bool getIsLocked();
    pub void lock();

    pub ~UniformAttributesList();
};
};
