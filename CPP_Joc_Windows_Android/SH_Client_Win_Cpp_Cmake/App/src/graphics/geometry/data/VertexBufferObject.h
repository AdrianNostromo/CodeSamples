#pragma once

#include <base/gh.h>
#include "BufferObject.h"
#include <base/list/CompositesBuffer.h>
#include <memory>

namespace graphics {
    class VertexAttributesList;
    class ShaderProgram;
};

namespace graphics {
class VertexBufferObject : public BufferObject {priv typedef BufferObject super; pub dCtor(VertexBufferObject);
    pub std::shared_ptr<VertexAttributesList> vertexAttributesList;

    pub explicit VertexBufferObject(
        std::shared_ptr<VertexAttributesList> vertexAttributesList, 
        UpdateMethod updateMethod,
        int initialCapacity = 0, int minCapacity = 0);
    
    pub std::shared_ptr<VertexAttributesList> getVertexAttributesList();

    pub void onBind(int slot, graphics::ShaderProgram* shader) override;
    pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

    pub void updateGLBufferIfNeeded(graphics::ShaderProgram* shader) override;

    prot void disposeMain() override;
    pub ~VertexBufferObject() override;
};
};
