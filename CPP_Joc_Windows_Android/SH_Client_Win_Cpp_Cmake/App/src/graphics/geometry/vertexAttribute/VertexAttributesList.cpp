#include "VertexAttributesList.h"
#include <graphics/shader/program/ShaderProgram.h>
#include "VertexAttribute.h"
#include <base/opengl/IGL.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace graphics;

VertexAttribute* const VertexAttributesList::Type::DiffuseColor = newt VertexAttribute(
    "a_diffuseColor", 
    IGL::UNSIGNED_BYTE/*dataType*/, 4/*count*/, VertexAttribute::NormalisationType::YES_floatConversion/*normalisationType*/
);
VertexAttribute* const VertexAttributesList::Type::Normal3D = newt VertexAttribute(
    "a_normal3D",
    IGL::FLOAT/*dataType*/, 3/*count*/, VertexAttribute::NormalisationType::NO_floatConversion/*normalisationType*/
);
VertexAttribute* const VertexAttributesList::Type::Position2D = newt VertexAttribute(
    "a_position2D",
    IGL::FLOAT/*dataType*/, 2/*count*/, VertexAttribute::NormalisationType::NO_floatConversion/*normalisationType*/,
    true/*mustExistInShader*/
);
VertexAttribute* const VertexAttributesList::Type::Position3D = newt VertexAttribute(
    "a_position3D",
    IGL::FLOAT/*dataType*/, 3/*count*/, VertexAttribute::NormalisationType::NO_floatConversion/*normalisationType*/,
    true/*mustExistInShader*/
);
VertexAttribute* const VertexAttributesList::Type::ModelAndNormalTransformIndex = newt VertexAttribute(
    "a_modelTransformIndex",
    IGL::UNSIGNED_INT/*dataType*/, 1/*count*/, VertexAttribute::NormalisationType::NO_integerType/*normalisationType*/
);
VertexAttribute* const VertexAttributesList::Type::DiffuseTextureIndex = newt VertexAttribute(
    "a_diffuseTextureIndex",
    IGL::INT/*dataType*/, 1/*count*/, VertexAttribute::NormalisationType::NO_integerType/*normalisationType*/
);
VertexAttribute* const VertexAttributesList::Type::MaterialIndex = newt VertexAttribute(
    "a_materialIndex",
    IGL::FLOAT/*dataType*/, 1/*count*/, VertexAttribute::NormalisationType::NO_floatConversion/*normalisationType*/
);
VertexAttribute* const VertexAttributesList::Type::TexCoord0 = newt VertexAttribute(
    "a_texCoord0",
    IGL::FLOAT/*dataType*/, 2/*count*/, VertexAttribute::NormalisationType::NO_floatConversion/*normalisationType*/
);

VertexAttribute* const VertexAttributesList::Type::Padding1 = newt VertexAttribute(
    "a_padding1",
    IGL::UNSIGNED_BYTE/*dataType*/, 1/*count*/, VertexAttribute::NormalisationType::NO_integerType/*normalisationType*/,
    false/*mustExistInShader*/, true/*isPadding*/
);
VertexAttribute* const VertexAttributesList::Type::Padding2 = newt VertexAttribute(
    "a_padding2",
    IGL::UNSIGNED_BYTE/*dataType*/, 2/*count*/, VertexAttribute::NormalisationType::NO_integerType/*normalisationType*/,
    false/*mustExistInShader*/, true/*isPadding*/
);
VertexAttribute* const VertexAttributesList::Type::Padding3 = newt VertexAttribute(
    "a_padding3",
    IGL::UNSIGNED_BYTE/*dataType*/, 3/*count*/, VertexAttribute::NormalisationType::NO_integerType/*normalisationType*/,
    false/*mustExistInShader*/, true/*isPadding*/
);

VertexAttributesList::VertexAttributesList() {
    //void
}

unsigned int VertexAttributesList::getStride() {
    return stride;
};

unsigned int VertexAttributesList::getTypesBitMask() {
    return typesBitMask;
};

bool VertexAttributesList::hasAll(long typeBitMask) {
    return ((typesBitMask & typeBitMask) == typeBitMask) ? true : false;
}

void VertexAttributesList::pushAttribute(VertexAttribute* attribute) {
    if (isLocked) {
        // Already locked, editing is not allowed any more.
        throw LogicException(LOC);
    }

    attributesList.appendDirect(attribute);
    stride += attribute->size;
    typesBitMask |= attribute->typeBitGroup;
}

int VertexAttributesList::getByteOffset(VertexAttribute* attribute) {
    if (!isLocked) {
        // This feature requires locked mode.
        throw LogicException(LOC);
    }

    int byteOffset = 0;
    bool isAttributeFound = false;
    for (int i = 0; i < attributesList.size(); i++) {
        VertexAttribute* entry = attributesList.getDirect(i);

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

void VertexAttributesList::configureInShader(ShaderProgram* shader) {
    if (shader == nullptr) {
        // Shader required.
        throw LogicException(LOC);
    }

    unsigned int offset = 0;

    if ((stride % 4) != 0) {
        // Stride must be 4_bytes aligned. This is a graphics card requirement.
        throw LogicException(LOC);
    }

    for(int i=0; i< attributesList.size(); i++) {
        VertexAttribute& attribute = *attributesList.getDirect(i);

        if (attribute.isPadding) {
            continue;
        }

        int attribLocation = shader->getAttribLocationOptional(attribute.alias);
        if(attribLocation >= 0) {
            // Used by the shader.
            GLUtil::gl->enableVertexAttribArray(attribLocation);
            if (attribute.normalisationType == VertexAttribute::NormalisationType::NO_integerType) {
                GLUtil::gl->vertexAttribIPointer(
                    attribLocation,
                    attribute.count, attribute.dataType,
                    getStride(), (const void*)(size_t)offset
                );
            } else {
                GLUtil::gl->vertexAttribPointer(
                    attribLocation,
                    attribute.count, attribute.dataType,
                    attribute.normalisationType == VertexAttribute::NormalisationType::YES_floatConversion ? IGL::TRUE : IGL::FALSE,
                    getStride(), (const void*)(size_t)offset
                );
            }
        } else if (attribute.mustExistInShader) {
            throw LogicException(LOC);
        }

        offset += attribute.size;
    }
}

void VertexAttributesList::removeShaderConfiguration(ShaderProgram* shader) {
    for(int i=0; i< attributesList.size(); i++) {
        VertexAttribute& attribute = *attributesList.getDirect(i);

        if (attribute.isPadding) {
            continue;
        }

        int attribLocation = shader->getAttribLocationOptional(attribute.alias);
        if(attribLocation >= 0) {
            // Used by the shader.
            GLUtil::gl->disableVertexAttribArray(attribLocation);
        } else if (attribute.mustExistInShader) {
            throw LogicException(LOC);
        }
    }
}

bool VertexAttributesList::getIsLocked() {
    return isLocked;
}

void VertexAttributesList::lock() {
    if (isLocked) {
        // Already locked.
        throw LogicException(LOC);
    }
    if ((stride % 4) != 0) {
        // Stride must be a multiple of 4. This is a gl memory requirement.
        throw LogicException(LOC);
    }
    
    isLocked = true;
}

VertexAttributesList::~VertexAttributesList() {
    for (int i = attributesList.size() - 1; i >= 0; i--) {
        const VertexAttribute* attribute = *attributesList.getPointer(i);

        attributesList.remove(i);
    }
}
