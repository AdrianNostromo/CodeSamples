#pragma once

#include <base/opengl/util/DataGLResource.h>
#include <base/list/CompositesBuffer.h>
#include <memory>
#include <base/opengl/IGL.h>

namespace graphics {
    class VertexAttributesList;
    class ShaderProgram;
    class IGLBufferChangesTracker;
};

namespace graphics {
class BufferObject : public DataGLResource {priv typedef DataGLResource super; pub dCtor(BufferObject);
    pub enum UpdateMethod{ SingleRegion, IndividualRegions };
    priv UpdateMethod updateMethod;

    priv GLenum glBindTarget;

    // This buffer count and stride must be locked because the video_memory block is created to this size and that block size should never have to change.
    prot CompositesBuffer buffer;
    
    priv IGLBufferChangesTracker* bufferChangesTracker = nullptr;

    // Used for fast static_casts to the subclasses.
    pub void* rawSubclassPointer = nullptr;

    pub explicit BufferObject(
        BindLocations::Config* bindLocation, GLenum glBindTarget,
        unsigned int stride, 
        UpdateMethod updateMethod,
        int initialCapacity = 0, int minCapacity = 0);
    prot void createMain() override;

    pub CompositesBuffer& getBuffer();

    prot void onLoadToGl() override;
    prot void onUnloadFromGl() override;

    // This should not be caled for buffers, use the regions invalidation function below instead.
    pub void invalidateData() override;
    pub void invalidateUsedRegion();
    pub void invalidateRegion(int entriesOffset, int entriesCount);

    prot void onValidateData() override;

    pub void onBind(int slot, graphics::ShaderProgram* shader) override;
    pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

    prot void disposeMain() override;
    pub ~BufferObject() override;
};
};
