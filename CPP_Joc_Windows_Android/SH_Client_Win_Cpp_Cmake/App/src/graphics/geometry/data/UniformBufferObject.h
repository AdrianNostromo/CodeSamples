#pragma once

#include <base/gh.h>
#include "BufferObject.h"
#include <base/list/CompositesBuffer.h>
#include <memory>

namespace graphics {
    class UniformAttributesList;
    class ShaderProgram;
};

//https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt
// Uniform buffer objects seem to be the same as uniform_blocks in the shader. The same limitation seems to apply (the size limit).
// This extension introduces the concept of a group of GLSL uniforms known as a "uniform block", and the API mechanisms to store "uniform blocks" in GL buffer objects.

namespace graphics {
class UniformBufferObject : public BufferObject {priv typedef BufferObject super; pub dCtor(UniformBufferObject);
    pub std::shared_ptr<UniformAttributesList> uniformAttributesList;
    
    pub explicit UniformBufferObject(
        std::shared_ptr<UniformAttributesList> uniformAttributesList,
        UpdateMethod updateMethod,
        int initialCapacity = 0, int minCapacity = 0);
    
    pub std::shared_ptr<UniformAttributesList> getUniformAttributesList();

    pub void onBind(int slot, graphics::ShaderProgram* shader) override;
    pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

    prot void disposePost() override;
    pub ~UniformBufferObject() override;
};
};
