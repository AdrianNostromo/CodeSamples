#include "VertexBufferObject.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

VertexBufferObject::VertexBufferObject(
    std::shared_ptr<VertexAttributesList> vertexAttributesList,
    UpdateMethod updateMethod,
    int initialCapacity, int minCapacity)
    : super(
        &BindLocations::ARRAY_BUFFER/*bindLocationIndex*/, IGL::ARRAY_BUFFER/*glBindTarget*/,
        vertexAttributesList->getStride()/*stride*/,
        updateMethod,
        initialCapacity/*initialCapacity*/, minCapacity/*minCapacity*/
    ),
    vertexAttributesList(vertexAttributesList)
{
    rawSubclassPointer = this;

    if (!vertexAttributesList->getIsLocked()) {
        // The vertex attributes list must be locked before use.
        throw LogicException(LOC);
    }
}

std::shared_ptr<VertexAttributesList> VertexBufferObject::getVertexAttributesList() {
    return vertexAttributesList;
};

void VertexBufferObject::onBind(int slot, graphics::ShaderProgram* shader) {
    super::onBind(slot, shader);

    if (shader != nullptr) {
        // Add a shader check because there is a initial bind call without a shader. That bind is used to allocate buffer vram and there is no shader at that time.
        vertexAttributesList->configureInShader(shader);
    }
}

void VertexBufferObject::updateGLBufferIfNeeded(graphics::ShaderProgram* shader) {
    super::updateGLBufferIfNeeded(shader);

    //void
}

void VertexBufferObject::onUnbind(int slot, graphics::ShaderProgram* shader) {
    if (shader != nullptr) {
        // Add a shader check because there is a initial bind call without a shader. That bind is used to allocate buffer vram and there is no shader at that time.
        vertexAttributesList->removeShaderConfiguration(shader);
    }

    super::onUnbind(slot, shader);
}

void VertexBufferObject::disposeMain() {
    if (vertexAttributesList != nullptr) {
        vertexAttributesList = nullptr;
    }

    super::disposeMain();
}

VertexBufferObject::~VertexBufferObject() {
    //void
}
