#pragma once

#include <base/gh.h>
#include <assert.h>
#include <base/list/ArrayList.h>
#include <string>
#include <base/opengl/util/GLUtil.h>

namespace graphics {
    class VertexAttribute;
    class ShaderProgram;
};

namespace graphics {
class VertexAttributesList {pub dCtor(VertexAttributesList);
    pub class Type {
        // These are example and can be re-instantiated. The alias and TypeBitGroup are the only check variables.

        pub static VertexAttribute* const DiffuseColor;
        pub static VertexAttribute* const Normal3D;
        pub static VertexAttribute* const Position2D;
        pub static VertexAttribute* const Position3D;
        pub static VertexAttribute* const ModelAndNormalTransformIndex;
        pub static VertexAttribute* const DiffuseTextureIndex;
        pub static VertexAttribute* const MaterialIndex;
        pub static VertexAttribute* const TexCoord0;

        pub static VertexAttribute* const Padding1;
        pub static VertexAttribute* const Padding2;
        pub static VertexAttribute* const Padding3;
    };

    // Unmanaged.
    priv ArrayList<VertexAttribute*> attributesList{};

    priv unsigned int stride = 0;
    priv long typesBitMask = 0;

    // This is used to make this list reusable after it is created.
    priv bool isLocked = false;

    pub VertexAttributesList();

    pub unsigned int getStride();
    pub unsigned int getTypesBitMask();

    pub bool hasAll(long typeBitMask);
    pub void pushAttribute(VertexAttribute* attribute);

    pub int getByteOffset(VertexAttribute* attribute);

    pub void configureInShader(ShaderProgram* shader);
    pub void removeShaderConfiguration(ShaderProgram* shader);

    pub bool getIsLocked();
    pub void lock();

    pub ~VertexAttributesList();
};
};
