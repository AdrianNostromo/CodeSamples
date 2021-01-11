#include "UniformAttributesList.h"
#include <graphics/shader/program/ShaderProgram.h>
#include "UniformAttribute.h"
#include <base/opengl/IGL.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace graphics;

UniformAttribute* const UniformAttributesList::Type::projViewMatrix = newt UniformAttribute(
    "projViewMatrix", 
    IGL::FLOAT/*dataType*/, 4/*column*/ * 4/*row*//*count in bytes*/
);
UniformAttribute* const UniformAttributesList::Type::modelTransform = newt UniformAttribute(
    "modelTransform", 
    IGL::FLOAT/*dataType*/, 4/*column*/ * 4/*row*//*count in bytes*/
);
UniformAttribute* const UniformAttributesList::Type::modelNormalTransform = newt UniformAttribute(
    "modelNormalTransform", 
    IGL::FLOAT/*dataType*/, 3/*columns*/ * (3/*rows*/ + 1/*float_padding*/)/*count in bytes*/
);
UniformAttribute* const UniformAttributesList::Type::uvec4 = newt UniformAttribute(
    "uvec4",
    IGL::UNSIGNED_INT/*dataType*/, 4/*componenets*//*count in bytes*/
);

UniformAttribute* const UniformAttributesList::Type::Padding1 = newt UniformAttribute(
    "a_padding1",
    IGL::UNSIGNED_BYTE/*dataType*/, 1/*count*/,
    false/*mustExistInShader*/, true/*isPadding*/
);
UniformAttribute* const UniformAttributesList::Type::Padding2 = newt UniformAttribute(
    "a_padding2",
    IGL::UNSIGNED_BYTE/*dataType*/, 2/*count*/,
    false/*mustExistInShader*/, true/*isPadding*/
);
UniformAttribute* const UniformAttributesList::Type::Padding3 = newt UniformAttribute(
    "a_padding3",
    IGL::UNSIGNED_BYTE/*dataType*/, 3/*count*/,
    false/*mustExistInShader*/, true/*isPadding*/
);
UniformAttribute* const UniformAttributesList::Type::Padding4 = newt UniformAttribute(
    "a_padding4",
    IGL::UNSIGNED_BYTE/*dataType*/, 4/*count*/,
    false/*mustExistInShader*/, true/*isPadding*/
);

UniformAttributesList::UniformAttributesList() {
    //void
}

unsigned int UniformAttributesList::getStride() {
    return stride;
};

unsigned int UniformAttributesList::getTypesBitMask() {
    return typesBitMask;
};

bool UniformAttributesList::hasAll(long typeBitMask) {
    return ((typesBitMask & typeBitMask) == typeBitMask) ? true : false;
}

void UniformAttributesList::pushAttribute(UniformAttribute* attribute) {
    if (isLocked) {
        // Already locked, editing is not allowed any more.
        throw LogicException(LOC);
    }

    attributesList.appendDirect(attribute);
    stride += attribute->size;
    typesBitMask |= attribute->typeBitGroup;
}

int UniformAttributesList::getByteOffset(UniformAttribute* attribute) {
    if (!isLocked) {
        // This feature requires locked mode.
        throw LogicException(LOC);
    }

    int byteOffset = 0;
    bool isAttributeFound = false;
    for (int i = 0; i < attributesList.size(); i++) {
        UniformAttribute* entry = attributesList.getDirect(i);

        if (entry->typeBitGroup == attribute->typeBitGroup) {
            isAttributeFound = true;

            break;
        }

        byteOffset += entry->size;
    }
    if (!isAttributeFound) {
        // Attribute not found.
        throw LogicException(LOC);
    }

    return byteOffset;
}

bool UniformAttributesList::getIsLocked() {
    return isLocked;
}

void UniformAttributesList::lock() {
    if (isLocked) {
        // Already locked.
        throw LogicException(LOC);
    }
    if ((stride % 4) != 0) {
        // Stride must be a multiple of 4. This is a gl memory requirement.
        //asd_r;// more checks are required. vec3,vec4, mat4 require an alignment of 4 * 4.
        throw LogicException(LOC);
    }

    isLocked = true;
}

UniformAttributesList::~UniformAttributesList() {
    for (int i = attributesList.size() - 1; i >= 0; i--) {
        const UniformAttribute* attribute = *attributesList.getPointer(i);

        attributesList.remove(i);
    }
}
