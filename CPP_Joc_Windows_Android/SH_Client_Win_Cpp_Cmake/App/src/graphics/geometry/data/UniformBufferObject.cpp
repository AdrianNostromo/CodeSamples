#include "UniformBufferObject.h"
#include <base/opengl/util/GLUtil.h>
#include <graphics/geometry/uniformAttribute/UniformAttributesList.h>
#include <base/exceptions/LogicException.h>

using namespace graphics;

UniformBufferObject::UniformBufferObject(
    std::shared_ptr<UniformAttributesList> uniformAttributesList,
    UpdateMethod updateMethod,
    int initialCapacity, int minCapacity)
    : super(
        &BindLocations::UNIFORM_BUFFER/*bindLocationIndex*/, IGL::UNIFORM_BUFFER/*glBindTarget*/,
        uniformAttributesList->getStride()/*stride*/,
        updateMethod,
        initialCapacity/*initialCapacity*/, minCapacity/*minCapacity*/
    ),
    uniformAttributesList(uniformAttributesList)
{
    rawSubclassPointer = this;

    if (!uniformAttributesList->getIsLocked()) {
        // The uniform attributes list must be locked before use.
        throw LogicException(LOC);
    }
}

std::shared_ptr<UniformAttributesList> UniformBufferObject::getUniformAttributesList() {
    return uniformAttributesList;
};

void UniformBufferObject::onBind(int slot, graphics::ShaderProgram* shader) {
    super::onBind(slot, shader);

    //void
}

void UniformBufferObject::onUnbind(int slot, graphics::ShaderProgram* shader) {
    super::onUnbind(slot, shader);

    //void
}

void UniformBufferObject::disposePost() {
    if (uniformAttributesList != nullptr) {
        uniformAttributesList = nullptr;
    }

    super::disposePost();
}

UniformBufferObject::~UniformBufferObject() {
    //void
}
